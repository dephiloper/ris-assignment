#include <map>
#include "server.h"

int main() {
    Server server;
    server.mainLoop();
    return 0;
}

Server::Server(): netManager(5555) {
    std::cout << "Server started." << std::endl;
    netManager.start();
    listeners.insert(std::pair<std::type_index, std::unique_ptr<NetMessageHandler>>(typeid(LoginMessage), std::make_unique<LoginMessageHandler>(&netManager, &world)));
    listeners.insert(std::pair<std::type_index, std::unique_ptr<NetMessageHandler>>(typeid(LogoutMessage), std::make_unique<LogoutMessageHandler>(&world)));
    listeners.insert(std::pair<std::type_index, std::unique_ptr<NetMessageHandler>>(typeid(InputMessage), std::make_unique<InputMessageHandler>(&playerInputs)));
    Potion p;
    p.position = {frand() * Tile::SIZE - Tile::SIZE/2, 0.25, frand() * Tile::SIZE - Tile::SIZE/2};
    p.isActive = true;
    world.potion = p;
}

void Server::mainLoop() {
    while(isRunning) {
        auto currentFrame = currentMillis();
        auto deltaTime = (currentFrame - lastFrame) / 1e6f;
        processMessages();
        updatePlayers(deltaTime);
        updatePotions(deltaTime);
        publishWorld();

        auto timeLeft = currentFrame + (1e6 / 60) - currentMillis();
        usleep(timeLeft < 0 ? 0 : timeLeft);
        if (timeLeft <= 0) std::cout << "\033[1;33mWarning:\033[0m Server doing to much work (time left: " << timeLeft << "us)" << std::endl;
        lastFrame = currentFrame;
    }
    netManager.stop();
}

void Server::processMessages() {
    auto queueCount = netManager.queueIn.size();
    // process only currently received messages
    for (auto i = 0; i < queueCount; i++) {
        auto msg = netManager.queueIn.pop();
        listeners.at(typeid(*msg))->handle(*msg);
    }
}

void Server::updatePlayers(float deltaTime) {
    for(auto& [id, p] : world.players) {
        checkForRespawn(&p);
        
        auto input = playerInputs.at(id);
        float velocity = Player::MOVEMENT_SPEED * deltaTime;
        auto loc = Tile::positionToTileLocation(p.position);
        p.front = input.front;

        // calculate front and right vectors
        glm::vec3 front = glm::normalize(glm::vec3(p.front.x, 0, p.front.z));
        glm::vec3 right = glm::normalize(glm::cross(front, WORLD_UP));

        glm::vec3 direction(0);

        if ((input.direction & FORWARD) == FORWARD)
            direction += front;
        if ((input.direction & BACKWARD) == BACKWARD)
            direction -= front;
        if ((input.direction & RIGHT) == RIGHT)
            direction += right;
        if ((input.direction & LEFT) == LEFT)
            direction -= right;

        if (input.shoot) {
            RayCast ray = shootAndCollide(static_cast<glm::vec3>(p.position), static_cast<glm::vec3>(p.front), id);
            Laser laser;
            laser.origin = Vector3::from(ray.origin - glm::normalize(glm::cross(right, front)) * (Player::SCALE / 10.0f));
            laser.target = ray.hit ? Vector3::from(ray.worldIntersection) : Vector3::from(ray.origin + ray.direction * Tile::SIZE);

            world.lasers.push_back(laser);
            if (!ray.intersectibleId.empty()) {
                Player* p = &world.players[ray.intersectibleId];
                p->hitPoints.push_back(Vector3::from(ray.modelIntersection));
                p->respawn = true;
                if (p->hasPotion) {
                    world.potion.position = { frand() * Tile::SIZE * 2 - Tile::SIZE, 0.4f, frand() * Tile::SIZE * 2 -  Tile::SIZE };
                    world.potion.isActive = true;
                    p->hasPotion = false;
                }
            }
        }

        if (direction != glm::vec3(0))
            direction = glm::normalize(direction) * velocity;

        // --- generate tiles for player ---

        auto currentLocation = Tile::positionToTileLocation(p.position);

        // player is new or player is entered new tile
        if (playerLocations.count(id) == 0 || playerLocations.at(id) != currentLocation) {
            playerLocations[id] = currentLocation;
            for (auto x = currentLocation.first-1; x <= currentLocation.first+1; x++) {
                for (auto z = currentLocation.second-1; z <= currentLocation.second+1; z++) {
                    auto location = std::pair(x,z);
                    // create tile only when it is non existent
                    if (globalTiles.count(location) == 0) {
                        std::vector<Vector3> playerPositions;
                        std::transform(world.players.begin(), world.players.end(), std::back_inserter(playerPositions), [](const std::pair<std::string, Player> &element) { return element.second.position; });
                        globalTiles[location] = Tile::generateNewTile(location, playerPositions);
                    }
                }
            }
            removeObsoleteTiles(); // TODO: does not get called when player disconnects!
            p.enteredNewLocation = true;
        }

        p.position = Vector3::from(moveAndSlide(static_cast<glm::vec3>(p.position), direction));
        // TODO for defining a fixed map size and looping it on x and z
        //p.position.x = loopValue(p.position.x, -Tile::SIZE*2.5f, Tile::SIZE*2.5f);
        //p.position.z = loopValue(p.position.z, -Tile::SIZE*2.5f, Tile::SIZE*2.5f);

        if (world.potion.isActive && Collision::squareCircleCollision(
            glm::vec2(world.potion.position.x, world.potion.position.z),
            glm::vec2(p.position.x, p.position.z), Potion::COLLISION_RADIUS / 2.0f, Player::SCALE / 2.0f)) {
                p.hasPotion = true;
                world.potion.isActive = false;
        }
    }
}

void Server::updatePotions(float deltaTime) {
    if (world.players.size() == 0 || !world.potion.isActive) return;

    glm::vec3 target = static_cast<glm::vec3>(world.players.begin()->second.position);

    auto location = Tile::positionToTileLocation(Vector3::from(world.potion.position));
    std::vector<Tile> tileArea = Tile::calculateTileArea(location, globalTiles);
    std::vector<Obstacle> obstacles;
    for (const auto& tile : tileArea)
        obstacles.insert(obstacles.end(), tile.obstacles.begin(), tile.obstacles.end());

    auto avoidance = world.potion.collisionAvoidance(obstacles);
    auto steering = world.potion.wander() + avoidance;
    steering = truncate(steering, world.potion.MAX_FORCE);
    world.potion.velocity = truncate(world.potion.velocity + steering, world.potion.MAX_SPEED);
    world.potion.position += Vector3::from(world.potion.velocity * deltaTime);
    world.potion.position.y = (1 + glm::sin(currentMillis() / 1e6 * 4.f)) / 8.0f + .25f; // moving up and down between 0.25 and 0.5
}

RayCast Server::shootAndCollide(const glm::vec3& origin, const glm::vec3& direction, const std::string& playerId) {
    RayCast ray(origin, direction);
    RayCast closestRay = ray;
    float closestDistance = std::numeric_limits<float>::max();

    // loop over all visible tiles to get the containing obstacles
    // check all obstacles for intersections
    for (auto const& t : Tile::calculateTileArea(playerLocations[playerId], globalTiles)) {
        for (auto const& obstacle : t.obstacles) {
            Obstacle o = obstacle;
            float distance = Collision::calculateParametricDistance(o, ray); // returns -1 when no intersection between ray and intersectable occures
            // when there is an intersection and the intersection point is closer to the ray origin
            if (distance != -1 && distance < closestDistance) {
                closestRay = ray;
                closestRay.hit = true;
                closestDistance = distance;
            }
        }
    }

    // check all players for intersections
    for (auto& [id, p] : world.players) {
        if (id == playerId) continue; // ignore own faces
        float distance = Collision::calculateParametricDistance(p, ray); // returns -1 when no intersection between ray and intersectable occures

        // when there is an intersection and the intersection point is closer to the ray origin
        if (distance != -1 && distance < closestDistance) {
            closestRay = ray;
            closestRay.hit = true;
            closestRay.intersectibleId = id;
            closestDistance = distance;
        }
    }
    return closestRay;
}

glm::vec3 Server::moveAndSlide(const glm::vec3& position, const glm::vec3& direction) {
    glm::vec3 destination = position;
    float newX = position.x + direction.x;
    float newZ = position.z + direction.z;
    glm::vec3 newPosX = glm::vec3(newX, 1, position.z);
    glm::vec3 newPosZ = glm::vec3(position.x, 1, newZ);

    if (!checkObstaclesForCollision(newPosX, Player::COLLISION_RADIUS))
        destination.x = newPosX.x;
    if (!checkObstaclesForCollision(newPosZ, Player::COLLISION_RADIUS))
        destination.z = newPosZ.z;

    return destination;
}

// player is a circle obstacles are squares
bool Server::checkObstaclesForCollision(const glm::vec3& destination, float playerRadius) {
    // TODO could be improved just look at the current tile and the destination neighbour tile
    auto location = Tile::positionToTileLocation(Vector3::from(destination));
    for (auto const& tile : Tile::calculateTileArea(location, globalTiles)) {
        for (auto const& obstacle : tile.obstacles) {
            auto square = glm::vec2(obstacle.position.x, obstacle.position.z);
            auto cirlce = glm::vec2(destination.x, destination.z);

            if (Collision::squareCircleCollision(square, cirlce, obstacle.radius, playerRadius))
                return true;
        }
    }
    return false;
}

void Server::publishWorld() {
    for(auto& [id, p] : world.players) {
        auto msg = std::make_shared<UpdateMessage>();
        msg->senderId = id;
        msg->players = this->world.players;
        msg->potion = this->world.potion;
        if (this->world.lasers.size() > 0)
            msg->lasers = this->world.lasers;

        if (p.enteredNewLocation) {
            auto loc = playerLocations.at(id);
            auto area = Tile::calculateLocationArea(loc);
            msg->tiles = Tile::calculateTileArea(playerLocations.at(id), globalTiles);
            p.enteredNewLocation = false;
        }
        netManager.queueOut.push(msg);
    }
    this->world.lasers.clear();
}

void Server::checkForRespawn(Player* p) {
    if (!p->respawn) return;
    
    std::vector<Obstacle> obstacles;
    for (const auto& [loc, tile] : globalTiles) {
        std::transform(tile.obstacles.begin(), tile.obstacles.end(), std::back_inserter(obstacles), [](Obstacle o) { return o; });
    }

    bool positionOccupied = true;
    glm::vec2 randPos;
    while (positionOccupied) {
        positionOccupied = false;
        randPos = { frand() * Tile::SIZE * 2 - Tile::SIZE, frand() * Tile::SIZE * 2 -  Tile::SIZE };
        for (const auto& obst : obstacles) {
            if (glm::distance(randPos, glm::vec2(obst.position.x, obst.position.z)) < Player::COLLISION_RADIUS + obst.radius) {
                positionOccupied = true;
                break;
            }
        }
    }
    p->position = Vector3 {randPos.x, 0.4, randPos.y};
    p->respawn = false;
}

void Server::removeObsoleteTiles() {
    std::set<std::pair<int, int>> usedLocations;
    std::set<std::pair<int, int>> unusedLocations;
    // get all currently used tile locations of the world
    for (auto const& [id, p] : world.players) {
        auto locations = Tile::calculateLocationArea(Tile::positionToTileLocation(p.position));
        usedLocations.insert(locations.begin(), locations.end());
    }

    // remove all tiles except the currently used from the map
    for (auto it = globalTiles.cbegin(); it != globalTiles.cend();)
    {
        // dereference the iterator, access the first item which is the location and erase the item
        // from the global tiles when it is not used
        auto location = (*it).first;
        if (usedLocations.count(location) == 0) {
            unusedLocations.insert(location);
            it = globalTiles.erase(it);
        }
        else {
            ++it;
        }
    }
}
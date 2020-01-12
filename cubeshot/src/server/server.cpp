#include <map>
#include "server.h"


int main() {
    Server server;
    server.mainLoop();
    return 0;
}

Server::Server(): netManager(5555) {
    std::cout << "Server started." << std::endl;
    netManager.start(netManager);
    listeners.insert(std::pair<std::type_index, std::unique_ptr<NetMessageHandler>>(typeid(LoginMessage), std::make_unique<LoginMessageHandler>(&netManager, &world)));
    listeners.insert(std::pair<std::type_index, std::unique_ptr<NetMessageHandler>>(typeid(LogoutMessage), std::make_unique<LogoutMessageHandler>(&world)));
    listeners.insert(std::pair<std::type_index, std::unique_ptr<NetMessageHandler>>(typeid(InputMessage), std::make_unique<InputMessageHandler>(&playerInputs)));
    //Potion p;
    //p.position = Vector3{0.0, 0.5, 0.0};
    //world.potions.push_back(p);
}

void Server::mainLoop() {
    while(isRunning) {
        auto currentFrame = Constants::currentMillis();
        auto deltaTime = (currentFrame - lastFrame) / 1e6f;
        processMessages();

        updatePlayers(deltaTime);
        
        updatePotions(deltaTime);
        
        publishWorld();

        auto timeLeft = currentFrame + (1e6 / 60) - Constants::currentMillis();
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

        auto input = playerInputs.at(id);
        float velocity = MOVEMENT_SPEED * deltaTime;

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
            laser.origin = Vector3::from(ray.origin - glm::normalize(glm::cross(right, front)) * (PLAYER_SCALE / 10.0f));
            laser.target = ray.hit ? Vector3::from(ray.worldIntersection) : Vector3::from(ray.origin + ray.direction * TILE_SIZE);

            world.lasers.push_back(laser);
            if (!ray.intersectableId.empty()) {
                world.players[ray.intersectableId].position = { rand() % (int)TILE_SIZE - TILE_SIZE/2.0f, 0.4f, rand() % (int)TILE_SIZE - TILE_SIZE/2.0f };
                world.players[ray.intersectableId].hitPoints.push_back(Vector3::from(ray.modelIntersection));
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
                        globalTiles[location] = Tile::generateNewTile(location);
                        Potion p;
                        auto pos = Tile::tileLocationToPosition(location);
                        p.position = Vector3 {pos.x + rand() % (int)TILE_SIZE - TILE_SIZE/2.0f, 0.5f, pos.z + rand() % (int)TILE_SIZE - TILE_SIZE/2.0f };
                        world.potions.push_back(p);
                    }
                }
            }
            removeObsoleteTiles(); // TODO: does not get called when player disconnects!
            p.enteredNewLocation = true;
        }

        p.position = Vector3::from(moveAndSlide(static_cast<glm::vec3>(p.position), direction));
    }
}

void Server::updatePotions(float deltaTime) {
    if (world.players.size() == 0) return;

    glm::vec3 target = static_cast<glm::vec3>(world.players.begin()->second.position);

    for(auto& potion : world.potions) {
        potion.position += Vector3::from(potion.seek(target));
        //std::cout << "steer " << Vector3::from(potion.seek(target)) << std::endl;
        //std::cout << potion.position << std::endl;
        potion.position.y = (1 + glm::sin(Constants::currentMillis() / 1e6 * 4.f)) / 4.0f + .25f; // moving up and down between 0.25 and 0.75
    }
}

template <typename CharT, typename TraitsT, glm::length_t L, typename T, glm::qualifier Q>
auto& operator<< (std::basic_ostream<CharT, TraitsT>& os, const glm::vec<L, T, Q>& v)
{
    os << '(' << v.x;
    for (auto i = 1ul; i < L; ++i)
        os << ", " << *(&v.x + i);
    os << ')';
    return os;
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
            o.position = Vector3{o.position.x + t.position.x, o.position.y, o.position.z + t.position.z};
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
            closestRay.intersectableId = id;
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

    if (!circleSquareCollision(newPosX, COLLISION_RADIUS))
        destination.x = newPosX.x;
    if (!circleSquareCollision(newPosZ, COLLISION_RADIUS))
        destination.z = newPosZ.z;
    
    return destination;
}

// player is a circle obstacles are squares
bool Server::circleSquareCollision(const glm::vec3& destination, float playerRadius) {
    // TODO could be improved just look at the current tile and the destination neighbour tile
    auto location = Tile::positionToTileLocation(Vector3::from(destination));
    for (auto const& tile : Tile::calculateTileArea(location, globalTiles)) {
        for (auto const& obstacle : tile.obstacles) {
            float testX = destination.x, testZ = destination.z;
            Vector3 globalPos = obstacle.position + tile.position;
            
            if (destination.x < globalPos.x - obstacle.radius)       testX = globalPos.x - obstacle.radius; // left edge
            else if (destination.x > globalPos.x + obstacle.radius)  testX = globalPos.x + obstacle.radius; // right edge
            if (destination.z < globalPos.z - obstacle.radius)       testZ = globalPos.z - obstacle.radius; // top edge
            else if (destination.z > globalPos.z + obstacle.radius)  testZ = globalPos.z + obstacle.radius; // bottom edge

            float distance = glm::distance(glm::vec2(destination.x, destination.z), glm::vec2(testX, testZ));
            
            if (distance <= playerRadius) return true;
        }
    }
    return false;
}

void Server::publishWorld() {
    for(auto& [id, p] : world.players) {
        auto msg = std::make_shared<UpdateMessage>();
        msg->senderId = id;
        msg->players = this->world.players;
        msg->potions = this->world.potions;
        if (this->world.lasers.size() > 0)
            msg->lasers = this->world.lasers;

        if (p.enteredNewLocation) {
            msg->tiles = Tile::calculateTileArea(playerLocations.at(id), globalTiles);
            p.enteredNewLocation = false;
        }
        netManager.queueOut.push(msg);
    }
    this->world.lasers.clear();
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

    removeObsoletePotions(unusedLocations);
}

void Server::removeObsoletePotions(const std::set<std::pair<int, int>>& unusedLocations) {
    int x = 0;
    for (auto it = world.potions.cbegin(); it != world.potions.cend();) {
        auto potion = (*it);
        auto location = Tile::positionToTileLocation(potion.position);

        if (unusedLocations.count(location) != 0) {
            it = world.potions.erase(it);
            x++;
        }
        else {
            ++it;
        }
    }
}
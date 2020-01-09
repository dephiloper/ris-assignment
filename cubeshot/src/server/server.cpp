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
}

void Server::mainLoop() {
    while(isRunning) {
        auto currentFrame = Constants::currentMillis();
        auto deltaTime = (currentFrame - lastFrame) / 1e6f;
        processMessages();

        updatePlayers(deltaTime);
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
            auto [playerId, hitPoint] = shootAndCollide(Vector3::toGlm(p.position), Vector3::toGlm(p.front), id);
            if (!playerId.empty())
                world.players[playerId].hitPoints.push_back(Vector3::from(hitPoint));
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
                    auto tilePos = std::pair(x,z);
                    // create tile only when it is non existent
                    if (globalTiles.count(tilePos) == 0) globalTiles[tilePos] = Tile::generateNewTile(tilePos);
                }
            }
            removeUnusedTiles(); // TODO: does not get called when player disconnects!
            p.enteredNewLocation = true;
        }

        p.position = Vector3::from(moveAndSlide(Vector3::toGlm(p.position), direction));
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

std::pair<std::string, glm::vec3> Server::shootAndCollide(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const std::string& playerId) {
    for (auto const& [id, p] : world.players) {
        if (id == playerId) continue; // ignore own faces

        // calculate translation and rotation matrix of the player
        glm::mat4 translation = glm::mat4(1.0f);
        translation = glm::translate(translation, Vector3::toGlm(p.position));
        glm::vec3 target = Vector3::toGlm(p.front);
        glm::vec3 r = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), target);
        glm::mat4 rotation = glm::inverse(glm::lookAt(glm::vec3(0), target, glm::vec3(0, 1, 0)));

        // translate and rotate the rayorigin/raydirection acording to the possible player to hit (player to hit at the origin of the coordinate system)
        glm::vec4 translatedOrigin = glm::inverse(translation * rotation) * glm::vec4(rayOrigin, 1.0f); // translate and rotate ray origin
        glm::vec4 rotatedDirection = glm::normalize(glm::inverse(rotation) * glm::vec4(rayDirection, 1.0f)); // just rotate the ray direction

        // check all possible face normals if an intersection is possible
        for (auto const& face : faceNormals) {
            float t;
            if (intersectPlane(-face, face * (PLAYER_SCALE / 2.0f), translatedOrigin, rotatedDirection, t)) {
                glm::vec3 s = (translatedOrigin + t * rotatedDirection);
                std::cout << s << std::endl;
                if (cube(s, (PLAYER_SCALE / 2.0f)) < 0.001) // apply distance function between intersection point an face plane
                    return std::pair(id, s); //(rayOrigin + t * rayDirection); // untransformed intersection point
            }
        }
    }
    return std::pair("", glm::vec3(0));
}

float Server::cube(const glm::vec3& p, float r) {
    glm::vec3 p2 = glm::abs(p) - r;
    //return glm::length(glm::max(p2, glm::vec3(0)));
    return glm::max(p2.x, glm::max(p2.y, p2.z));
}

bool Server::intersectPlane(const glm::vec3 &n, const glm::vec3 &p0, const glm::vec3 &l0, const glm::vec3 &l, float &t) 
{ 
    // assuming vectors are all normalized
    float denom = glm::dot(n, l); 
    if (denom > 1e-6) { 
        auto p0l0 = p0 - l0; 
        t = glm::dot(p0l0, n) / denom; 
        return (t >= 0); 
    } 
 
    return false; 
} 

glm::vec3 Server::moveAndSlide(const glm::vec3& position, const glm::vec3& direction) {
    glm::vec3 destination = position;
    float newX = position.x + direction.x;
    float newZ = position.z + direction.z;
    glm::vec3 newPosX = glm::vec3(newX, 1, position.z);
    glm::vec3 newPosZ = glm::vec3(position.x, 1, newZ);

    if (!checkForCollision(newPosX, COLLISION_RADIUS))
        destination.x = newPosX.x;
    if (!checkForCollision(newPosZ, COLLISION_RADIUS))
        destination.z = newPosZ.z;
    
    return destination;
}

bool Server::checkForCollision(const glm::vec3& destination, float playerRadius) {
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
        msg->players = this->world.players;
        msg->senderId = id;
        if (p.enteredNewLocation) {
            msg->tiles = Tile::calculateTileArea(playerLocations.at(id), globalTiles);
            p.enteredNewLocation = false;
        }
        netManager.queueOut.push(msg);
    }
}

void Server::removeUnusedTiles() {
    std::set<std::pair<int, int>> usedLocations;

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
        if (usedLocations.count((*it).first) == 0)
            it = globalTiles.erase(it);    // or "it = m.erase(it)" since C++11
        else
            ++it;
    }
}
#ifndef TILE_H
#define TILE_H

#include <nop/structure.h>
#include "vector3.h"
#include "obstacle.h"
#include "potion.h"
#include <map>

struct Tile {
public:
    static constexpr float SIZE = 20.0f;

    Vector3 position;
    Vector3 scale{SIZE, 0.2f, SIZE};
    std::vector<Obstacle> obstacles;

    static Tile generateNewTile(std::pair<int, int> location) {
        Tile tile;
        tile.position = Vector3{ location.first * tile.scale.x, -0.1, location.second * tile.scale.z };
        for (auto i = 0; i < (int)(SIZE*1.5f); i++) {
            Obstacle obstacle;
            obstacle.position = tile.position + Vector3{rand() % (int)SIZE - SIZE / 2.0f, 0.6f, rand() % (int)SIZE - SIZE / 2.0f};
            obstacle.front = Vector3{0.0, 0.0, 1.0};
            tile.obstacles.push_back(obstacle);
        }
        return tile;
    }

    static std::vector<std::pair<int, int>> calculateLocationArea(const std::pair<int,int>& location) {
        std::vector<std::pair<int, int>> locationArea;
        for (auto x = location.first-1; x <= location.first+1; x++) {
            for (auto z = location.second-1; z <= location.second+1; z++) {
            // TODO for defining a fixed map size and looping it on x and z
                //int loopedX = loopValue(x, -2, 2);
                //int loopedZ = loopValue(z, -2, 2);
                locationArea.push_back(std::pair<int,int>(x,z));
            }
        }

        return locationArea;
    }

    static std::pair<int,int> positionToTileLocation(const Vector3& position) {
        float x = position.x > 0 ? position.x + (SIZE/2) : position.x - (SIZE/2);
        float z = position.z > 0 ? position.z + (SIZE/2) : position.z - (SIZE/2);
        return std::pair<int,int>((int)(x / SIZE), (int)(z / SIZE));
    }

    static Vector3 tileLocationToPosition(const std::pair<int,int>& location) {
        return Vector3 {location.first * SIZE, 0.0f, location.second * SIZE};
    }

    static std::vector<Tile> calculateTileArea(const std::pair<int,int>& location, const std::map<std::pair<int, int>, Tile>& globalTiles) {
        return tilesFromLocations(calculateLocationArea(location), globalTiles);
    }

    static std::vector<Tile> tilesFromLocations(const std::vector<std::pair<int, int>>& locations, const std::map<std::pair<int, int>, Tile>& globalTiles) {
        std::vector<Tile> tiles;
        for (auto const& location : locations) {
            if (globalTiles.count(location) == 0) continue;
            tiles.push_back(globalTiles.at(location));
        }

        return tiles;
    }

    NOP_STRUCTURE(Tile, position, obstacles);
};

#endif //TILE_H
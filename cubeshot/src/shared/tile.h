#ifndef TILE_H
#define TILE_H

#include <nop/structure.h>
#include "vector3.h"
#include "obstacle.h"

const float TILE_SIZE = 20.0f;

struct Tile {
    Vector3 position;
    Vector3 scale{ TILE_SIZE, 0.2f, TILE_SIZE };
    std::vector<Obstacle> obstacles;

    static Tile generateNewTile(std::pair<int, int> location) {
        Tile tile;
        tile.position = Vector3{ location.first * tile.scale.x, -0.1, location.second * tile.scale.z };
        for (auto i = 0; i < 16; i++)
            tile.obstacles.push_back(Obstacle{ Vector3{ rand() % (int)TILE_SIZE - TILE_SIZE/2, 0.5f, rand() % (int)TILE_SIZE - TILE_SIZE/2 } });

        return tile;
    }

    static std::vector<std::pair<int, int>> calculateLocationArea(const std::pair<int,int>& location) {
        std::vector<std::pair<int, int>> locationArea;
        for (auto x = location.first-1; x <= location.first+1; x++) {
            for (auto z = location.second-1; z <= location.second+1; z++) {
                locationArea.push_back(std::pair<int,int>(x,z));
            }
        }

        return locationArea;
    }

    static std::pair<int,int> positionToTileLocation(const Vector3& position) {
        float x = 0, z = 0;
        x = position.x > 0 ? position.x + (TILE_SIZE/2) : position.x - (TILE_SIZE/2);
        z = position.z > 0 ? position.z + (TILE_SIZE/2) : position.z - (TILE_SIZE/2);
        return std::pair<int,int>((int)(x / TILE_SIZE), (int)(z / TILE_SIZE));
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
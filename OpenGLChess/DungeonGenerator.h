#pragma once
#include <vector>
#include "GameEntity.h"
#include "Shader.h"
#include "AssetLoader.h"
#include <memory>
#include <glm/glm.hpp>
#include <random>

// Define a simple struct to represent a grid cell
struct Cell {
    bool isWall; // True if the cell is a wall, false if it's a floor
};

class DungeonGenerator {
public:
    DungeonGenerator(int width, int height, int maxRooms, std::vector<GameEntity*>& entities, Shader* shader);

    void Generate();

private:
    int m_width;
    int m_height;
    int m_maxRooms;
    std::vector<std::vector<int>> m_grid;  // 2D grid representing the dungeon
    std::vector<GameEntity*>& m_entities;
    Shader* m_shader;

    std::unique_ptr<Mesh> cubeMesh;
    void InitializeGrid();
    void RandomWalkAlgorithm();
    void CreateRoom(int x, int y);
    void CreateCorridor(int x, int y);
    int CountAdjacentRooms(int x, int y);
};

DungeonGenerator::DungeonGenerator(int width, int height, int maxRooms, std::vector<GameEntity*>& entities, Shader* shader)
    : m_width(width), m_height(height), m_maxRooms(maxRooms), m_entities(entities), m_shader(shader) {
    cubeMesh = std::unique_ptr<Mesh>(AssetLoader::LoadMesh("cube3.m"));
    InitializeGrid();
    Generate();
}

void DungeonGenerator::InitializeGrid() {
    m_grid.resize(m_height, std::vector<int>(m_width, 0)); // 0 for walls
}

void DungeonGenerator::Generate() {
    srand(static_cast<unsigned int>(time(0))); // Seed for random number generation
    RandomWalkAlgorithm();
}

void DungeonGenerator::RandomWalkAlgorithm() {
    int x = m_width / 2;
    int y = m_height / 2;
    int roomsCreated = 0;

    std::vector<std::pair<int, int>> directions = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };

    auto isInsideGrid = [&](int nx, int ny) {
        return nx >= 1 && nx < m_width - 1 && ny >= 1 && ny < m_height - 1;
    };

    while (roomsCreated < m_maxRooms) {
        if (isInsideGrid(x, y)) {
            if (m_grid[y][x] == 0) {
                CreateRoom(x, y);
                roomsCreated++;
            }

            // Bias towards unexplored areas
            std::vector<std::pair<int, int>> possibleMoves;
            for (const auto& dir : directions) {
                int nx = x + dir.first;
                int ny = y + dir.second;
                if (isInsideGrid(nx, ny) && m_grid[ny][nx] == 0) {
                    possibleMoves.push_back(dir);
                }
            }

            if (!possibleMoves.empty()) {
                // Choose a random direction from possible unexplored directions
                auto dir = possibleMoves[rand() % possibleMoves.size()];
                x += dir.first;
                y += dir.second;
            }
            else {
                // If no unexplored areas, move randomly
                int direction = rand() % 4;
                x += directions[direction].first;
                y += directions[direction].second;
            }
        }
        else {
            // If out of bounds, move back to the center or a random valid position
            x = rand() % (m_width - 2) + 1;
            y = rand() % (m_height - 2) + 1;
        }
    }

    // Ensure connectivity by filling gaps with corridors
    for (int i = 1; i < m_height - 1; ++i) {
        for (int j = 1; j < m_width - 1; ++j) {
            if (m_grid[i][j] == 0 && CountAdjacentRooms(j, i) > 1) {
                CreateCorridor(j, i);
            }
        }
    }
}

int DungeonGenerator::CountAdjacentRooms(int x, int y) {
    int count = 0;
    if (m_grid[y - 1][x] == 1) count++;
    if (m_grid[y + 1][x] == 1) count++;
    if (m_grid[y][x - 1] == 1) count++;
    if (m_grid[y][x + 1] == 1) count++;
    return count;
}

void DungeonGenerator::CreateCorridor(int x, int y) {
    m_grid[y][x] = 1;
    GameEntity* corridorEntity = new GameEntity(cubeMesh.get(), m_shader);
    corridorEntity->SetPosition(glm::vec3(x * 2.0f, 0.0f, y * 2.0f));
    corridorEntity->Scale(1.0f, 1.0f, 1.0f);  // Adjust scale as needed
    m_entities.push_back(corridorEntity);
}

void DungeonGenerator::CreateRoom(int x, int y) {
    m_grid[y][x] = 1;  // Mark this cell as a room or corridor

    float roomSize = 2.0f;  // Define the size of each room; adjust as needed
    float wallThickness = 0.2f;  // Define the thickness of the walls

    // Create the four wall cubes for the room
    std::vector<glm::vec3> wallPositions = {
        glm::vec3(x * roomSize, 0.0f, y * roomSize - roomSize / 2.0f),  // North wall
        glm::vec3(x * roomSize, 0.0f, y * roomSize + roomSize / 2.0f),  // South wall
        glm::vec3(x * roomSize - roomSize / 2.0f, 0.0f, y * roomSize),  // West wall
        glm::vec3(x * roomSize + roomSize / 2.0f, 0.0f, y * roomSize)   // East wall
    };

    for (const auto& wallPos : wallPositions) {
        // Create the GameEntity for the wall cube
        GameEntity* wallEntity = new GameEntity(cubeMesh.get(), m_shader);  // Pass the shader used for rendering

        // Set the position of the wall entity in the world space
        wallEntity->SetPosition(wallPos);

        // Scale the wall to fit the dungeon cell size; the wall is thin and spans the room's width/height
        if (wallPos.x == x * roomSize) {  // North or South wall (Z-axis alignment)
            wallEntity->Scale(roomSize / 2.0f, roomSize / 2.0f, wallThickness);
        }
        else {  // West or East wall (X-axis alignment)
            wallEntity->Scale(wallThickness, roomSize / 2.0f, roomSize / 2.0f);
        }

        // Add the wall entity to the list of entities for rendering
        m_entities.push_back(wallEntity);
    }
}
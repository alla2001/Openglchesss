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

    void InitializeGrid();
    void RandomWalkAlgorithm();
    void CreateRoom(int x, int y);
};

DungeonGenerator::DungeonGenerator(int width, int height, int maxRooms, std::vector<GameEntity*>& entities, Shader* shader)
    : m_width(width), m_height(height), m_maxRooms(maxRooms), m_entities(entities), m_shader(shader) {
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

    while (roomsCreated < m_maxRooms) {
        if (x >= 1 && x < m_width - 1 && y >= 1 && y < m_height - 1) {
            if (m_grid[y][x] == 0) {
                CreateRoom(x, y);
                roomsCreated++;
            }

            int direction = rand() % 4;
            switch (direction) {
            case 0: y++; break;  // Move up
            case 1: y--; break;  // Move down
            case 2: x++; break;  // Move right
            case 3: x--; break;  // Move left
            }
        }
    }
}

void DungeonGenerator::CreateRoom(int x, int y) {
    m_grid[y][x] = 1;  // Mark this cell as a room or corridor

    // Generate the cube mesh for the room
    std::unique_ptr<Mesh> cubeMesh = std::unique_ptr<Mesh>(AssetLoader::LoadMesh("cube3.m"));

    // Create the GameEntity for the cube
    GameEntity* cubeEntity = new GameEntity(cubeMesh.get(), m_shader);  // Pass the shader used for rendering

    // Set the position of the cube entity in the world space
    float roomSize = 2.0f;  // Define the size of each room; adjust as needed
    cubeEntity->SetPosition(glm::vec3(x * roomSize, 0.0f, y * roomSize));  // Convert grid coordinates to world coordinates

    // Scale the cube to fit your dungeon cell size
    cubeEntity->Scale(roomSize, roomSize, roomSize);

    // Add the cube entity to the list of entities for rendering
    m_entities.push_back(cubeEntity);
}
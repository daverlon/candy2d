#pragma once

#include "../stdafx.h"
#include "../GameClasses/EntityManager.h"
#include "../GameClasses/GameCamera.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimatorComponent.h"
#include "../Components/ColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/EnemyAIComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/SkullBulletComponent.h"

// #include "../Components/HealthComponent.h"

#include "../Utils/GameMath.h"

template<typename ComponentType1, typename ComponentType2>
bool CheckCollision(Entity* entity1, Entity* entity2) {

    if ((entity1->GetComponent<ComponentType1>() != nullptr && entity2->GetComponent<ComponentType2>() != nullptr)
        || (entity1->GetComponent<ComponentType2>() != nullptr && entity2->GetComponent<ComponentType1>() != nullptr)) {
        return true;
    }

    return false;
}

// grid based collision system
// assumes the grid (world) starts at 0, 0

#define CELL_SIZE 10
#define WORLD_WIDTH 1500
#define WORLD_HEIGHT 1500
#define WORLD_X -200
#define WORLD_Y -200

class ColliderSystem {
private:
    EntityManager *_entityManager;
    GameCamera *_camera; // for debug rendering

public:
    // int cellKey = static_cast<int>(x / cellSize) + static_cast<int>(y / cellSize) * (worldWidth / cellSize);
    std::unordered_map<int, std::vector<Entity*>> _cells;

    inline int CellKey(int x, int y) {
        x = (x >= 0) ? 2 * x : -2 * x - 1;
        y = (y >= 0) ? 2 * y : -2 * y - 1;

        // Combine x and y into a unique key
        if (x >= y) {
            return x * x + x + y;
        } else {
            return y * y + x;
        }
    }

    inline std::vector<Entity*> GetEntitiesInCell(int x, int y) {
        if (_cells.find(CellKey(x, y)) != _cells.end())
            return _cells[CellKey(x, y)];
        // std::cout << "Invalid cell" << std::endl;
        return {};
    }

    inline void AddEntityToCell(int x, int y, Entity* entity) {
        int k = CellKey(x, y);

        if (_cells.find(k) == _cells.end()) {
            _cells[k] = std::vector<Entity*>();
        }

        _cells[k].push_back(entity);
    } 

    inline void RemoveEntityFromCell(int x, int y, Entity* entity) {
        int k = CellKey(x, y);
        if (_cells.find(k) != _cells.end()) {
            auto& cell = _cells[k];
            auto it = std::find(cell.begin(), cell.end(), entity);
            if (it != cell.end()) {
                cell.erase(it);
            }
        }
    }

    ColliderSystem(EntityManager *entityManager, GameCamera *camera) : 
    _entityManager(entityManager), 
    _camera(camera) {
        std::cout << "ColliderSystem()" << this << std::endl;

    }

    ~ColliderSystem() {
        std::cout << "~ColliderSystem()" << this << std::endl;
    }

    void Update(const float& dt);

    void Render(SDL_Renderer* renderer);
};
#pragma once

#include "../stdafx.h"
#include "../GameClasses/EntityManager.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimatorComponent.h"
#include "../Components/ColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/EnemyAIComponent.h"
#include "../Components/PlayerComponent.h"
// #include "../Components/HealthComponent.h"

#include "../Utils/GameMath.h"

// grid based collision system
// assumes the grid (world) starts at 0, 0

#define CELL_SIZE 20
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

    int CellKey(int x, int y) {
        x = (x >= 0) ? 2 * x : -2 * x - 1;
        y = (y >= 0) ? 2 * y : -2 * y - 1;

        // Combine x and y into a unique key
        if (x >= y) {
            return x * x + x + y;
        } else {
            return y * y + x;
        }
    }

    std::vector<Entity*> GetEntitiesInCell(int x, int y) {
        if (_cells.find(CellKey(x, y)) != _cells.end())
            return _cells[CellKey(x, y)];
        // std::cout << "Invalid cell" << std::endl;
        return {};
    }

    void AddEntityToCell(int x, int y, Entity* entity) {
        int k = CellKey(x, y);

        if (_cells.find(k) == _cells.end()) {
            _cells[k] = std::vector<Entity*>();
        }

        _cells[k].push_back(entity);
    } 

    void RemoveEntityFromCell(int x, int y, Entity* entity) {
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

    void Update(const float &dt) {

        // Entity* player = _entityManager->GetFirstEntityWithComponent<PlayerComponent>();
    
        _cells.clear();

        for (auto &entity : _entityManager->GetEntitiesWithComponent<ColliderComponent>()) {
            auto collider = entity->GetComponent<ColliderComponent>();
            assert(collider != nullptr);

            auto transform = entity->GetComponent<TransformComponent>();
            assert(transform != nullptr);

            const glm::vec4& bounds = collider->GetBounds();
            glm::vec2 position = transform->GetPosition();

            int minX = static_cast<int>((position.x + bounds.x) / CELL_SIZE);
            int minY = static_cast<int>((position.y + bounds.y) / CELL_SIZE);
            int maxX = static_cast<int>((position.x + bounds.x + bounds.z) / CELL_SIZE);
            int maxY = static_cast<int>((position.y + bounds.y + bounds.w) / CELL_SIZE);

            for (int x = minX; x <= maxX; ++x) {
                for (int y = minY; y <= maxY; ++y) {
                    AddEntityToCell(x, y, entity);
                    // std::cout << "Adding " << entity << " to cell (" << x << "," << y << "), k: " << CellKey(x, y) << std::endl;
                }
            }
        }

        for (int x = WORLD_X; x < WORLD_WIDTH / CELL_SIZE; ++x) {
            for (int y = WORLD_Y; y < WORLD_HEIGHT / CELL_SIZE; ++y) {

                std::vector<Entity*> entitiesInCell = GetEntitiesInCell(x, y);

                // int count = entitiesInCell.size();
                // if (count > 0) {
                //     std::cout << "(" << x << "," << y << "): " << count << std::endl;
                //     for (auto &e : entitiesInCell) {
                //         std::cout << e << std::endl;
                //     }
                //     std::cout << std::endl;
                // }


                if (entitiesInCell.size() > 1) {
                    // Check for collisions between entities in this cell
                    for (size_t i = 0; i < entitiesInCell.size(); ++i) {
                        auto entity1 = entitiesInCell[i];

                        auto transform1 = entity1->GetComponent<TransformComponent>();
                        auto collider1 = entity1->GetComponent<ColliderComponent>();

                        for (size_t j = i + 1; j < entitiesInCell.size(); ++j) {
                            auto entity2 = entitiesInCell[j];

                            if (entity1 == entity2) continue;

                            auto transform2 = entity2->GetComponent<TransformComponent>();
                            auto collider2 = entity2->GetComponent<ColliderComponent>();

                            auto pos1 = transform1->GetPosition();
                            auto pos2 = transform2->GetPosition();

                            auto bounds1 = collider1->GetBounds();
                            auto bounds2 = collider2->GetBounds();

                            auto rect1 = SDL_FRect{pos1.x + bounds1.x, pos1.y + bounds1.y, bounds1.z, bounds1.w};
                            auto rect2 = SDL_FRect{pos2.x + bounds2.x, pos2.y + bounds2.y, bounds2.z, bounds2.w};

                            // Check for collision between collider1 and collider2
                            
                            if (SDL_HasIntersectionF(&rect1, &rect2)) {

                            // std::cout << entity1 << RectToString(rect1) << 
                            //     " <-> " << entity2 << RectToString(rect2) << std::endl;
                            // player collision with enemy
                                // if (((entity1 == player && ent2->GetComponent<EnemyAIComponent>() != nullptr)) 
                                //     || (ent2 == player && ent1->GetComponent<EnemyAIComponent>() != nullptr)) {
                                //     std::cout << "Player damage!" << std::endl;
                                // }

                                if (collider1->GetIsTrigger() || collider2->GetIsTrigger()) {
                                    // std::cout << "Trigger collision between " << entity1 << " and " << entity2 << std::endl;
                                    if (entity1->GetComponent<SkullBulletComponent>() != nullptr
                                        && entity2->GetComponent<EnemyAIComponent>() != nullptr) {
                                        // todo: entity manager? it is not save to remove entities in the collision system
                                        // perhaps on this collision event, remove the health of both entities or something
                                        // or flag them to be removed then have an entity manager remove them safely
                                        entity1->FlagDeletion();
                                        entity2->FlagDeletion();
                                    }
                                    if (entity2->GetComponent<SkullBulletComponent>() != nullptr
                                        && entity1->GetComponent<EnemyAIComponent>() != nullptr) {
                                        entity1->FlagDeletion();
                                        entity2->FlagDeletion();
                                    }

                                }
                                else if (!collider1->GetIsTrigger() && !collider2->GetIsTrigger())
                                {
                                    // Calculate the overlap between the rectangles
                                    float overlapLeft = (rect1.x + rect1.w) - rect2.x;
                                    float overlapRight = (rect2.x + rect2.w) - rect1.x;
                                    float overlapTop = (rect1.y + rect1.h) - rect2.y;
                                    float overlapBottom = (rect2.y + rect2.h) - rect1.y;

                                    // Find the minimum overlap
                                    float minOverlapX = std::min(overlapLeft, overlapRight);
                                    float minOverlapY = std::min(overlapTop, overlapBottom);

                                    // Calculate separation vector
                                    glm::vec2 separation = glm::vec2(0.0f, 0.0f);

                                    if (minOverlapX < minOverlapY)
                                    {
                                        // Separate horizontally
                                        separation.x = overlapLeft < overlapRight ? -minOverlapX : minOverlapX;
                                    }
                                    else
                                    {
                                        // Separate vertically
                                        separation.y = overlapTop < overlapBottom ? -minOverlapY : minOverlapY;
                                    }

                                    // Apply separation to entities' positions
                                    if (!collider1->GetIsUnmoveable() && !collider2->GetIsUnmoveable())
                                    {
                                        // If both entities are movable, adjust the positions of both entities
                                        glm::vec2 newPos1 = pos1 + separation * 0.5f;
                                        glm::vec2 newPos2 = pos2 - separation * 0.5f;
                                        transform1->SetPosition(newPos1);
                                        transform2->SetPosition(newPos2);
                                    }
                                    else if (!collider1->GetIsUnmoveable())
                                    {
                                        // If the first entity is movable, adjust the position of the first entity only
                                        glm::vec2 newPos1 = pos1 + separation;
                                        transform1->SetPosition(newPos1);                                    
                                    }
                                    else if (!collider2->GetIsUnmoveable())
                                    {
                                        // If the second entity is movable, adjust the position of the second entity only
                                        glm::vec2 newPos2 = pos2 - separation;
                                        transform2->SetPosition(newPos2);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // (debug) render collision bounds
    void Render(SDL_Renderer *renderer) {
        // return;
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);


        // render player collision bounds
        Entity *player = _entityManager->GetFirstEntityWithComponent<PlayerComponent>();
        auto playerBounds = player->GetComponent<ColliderComponent>()->GetBounds();

        auto playerPos = player->GetComponent<TransformComponent>()->GetPosition() + glm::vec2(playerBounds.x, playerBounds.y);
        SDL_FRect playerRect = SDL_FRect{playerPos.x, playerPos.y, playerBounds.z, playerBounds.w};
        SDL_FRect playerRectScreen = {};
        _camera->RectWorldToScreen(&playerRect, &playerRectScreen);
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 30);
        SDL_RenderFillRectF(renderer, &playerRectScreen);

        // render collision entities (objects)
        for (auto &ent : _entityManager->GetEntitiesWithComponent<ColliderComponent>()) {

            auto entCollider = ent->GetComponent<ColliderComponent>();
            assert(entCollider != nullptr);

            auto entTransform = ent->GetComponent<TransformComponent>();
            assert(entTransform != nullptr);
            auto entPos = entTransform->GetPosition();

            auto entBounds = entCollider->GetBounds();

            SDL_FRect entRect = (SDL_FRect){entPos.x + entBounds.x, entPos.y + entBounds.y, entBounds.z, entBounds.w};
            SDL_FRect entRectScreen = {};
            _camera->RectWorldToScreen(&entRect, &entRectScreen);

            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 100);
            SDL_RenderFillRectF(renderer, &entRectScreen);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRectF(renderer, &entRectScreen);
        }

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // Adjust the color as needed

        float sx = static_cast<float>(WORLD_WIDTH);
        float sy = static_cast<float>(WORLD_HEIGHT);
        auto gridWorldBounds = _camera->WorldToScreen(glm::vec2(sx, sy));

        // Draw horizontal lines
        for (int y = WORLD_Y; y <= WORLD_HEIGHT; y += CELL_SIZE)
        {
            auto screen = _camera->WorldToScreen(glm::vec2(WORLD_X, static_cast<float>(y)));
            SDL_RenderDrawLineF(renderer, screen.x, screen.y, gridWorldBounds.x, screen.y);
        }

        for (int x = WORLD_X; x <= WORLD_WIDTH; x += CELL_SIZE)
        {
            auto screen = _camera->WorldToScreen(glm::vec2(static_cast<float>(x), WORLD_Y));
            SDL_RenderDrawLineF(renderer, screen.x, screen.y, screen.x, gridWorldBounds.y);
        }

        //#define MAX_ENTITIES 50

        // // Iterate over grid cells
        // for (int x = 0; x <= WORLD_WIDTH; x += CELL_SIZE) {
        //     for (int y = 0; y <= WORLD_HEIGHT; y += CELL_SIZE) {

        //         int entityCount = GetEntitiesInCell(x, y).size();
        //         std::cout << "(" << x << "," << y << "): " << entityCount << std::endl;

        //         Uint8 alpha = 255 - ((entityCount/MAX_ENTITIES) * 255);
        //         SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);

        //         auto size = static_cast<float>(CELL_SIZE);
        //         SDL_FRect worldCellRect = SDL_FRect{static_cast<float>(x), static_cast<float>(y), size, size};
        //         SDL_FRect screenCellRect = SDL_FRect{0, 0, 0, 0};
        //         _camera->RectWorldToScreen(&worldCellRect, &screenCellRect);

        //         SDL_RenderFillRectF(renderer, &screenCellRect);
        //     }
        // }
    }
};
#pragma once

#include "../stdafx.h"
#include "Entity.h"

#define SHOWDEBUG


// chatGPT!
class EntityManager {
private:
    std::unordered_map<std::type_index, std::unordered_set<Entity*>> _entityComponents;

public:
    EntityManager() {
#ifdef SHOWDEBUG
        std::cout << "EntityManager()" << std::endl;
#endif
    }

    ~EntityManager() {
        RemoveAllEntities();
    }

    void RemoveAllEntities() {
        #ifdef SHOWDEBUG
        std::cout << "~EntityManager()" << std::endl;
        std::cout << "[Entity Manager] Clearing entities container..." << std::endl;
#endif

        std::unordered_set<Entity*> deletedEntities;

        for (auto& pair : _entityComponents) {
            for (auto entity : pair.second) {
                if (deletedEntities.find(entity) == deletedEntities.end()) {
#ifdef SHOWDEBUG
                    std::cout << "[Entity Manager] Deleting entity " << entity << "..." << std::endl;
#endif
                    deletedEntities.insert(entity);
                    delete entity;
                }
            }
        }
#ifdef SHOWDEBUG
        std::cout << "_entityComponents size before: " << _entityComponents.size() << std::endl;
#endif
        _entityComponents.clear();
        // todo: check if deletedEntities should be cleared? it should be storing null pointers at this point anyway
#ifdef SHOWDEBUG
        std::cout << "_entityComponents size after:  " << _entityComponents.size() << std::endl;
        std::cout << "[Entity Manager] Finished deleting entities" << std::endl;
#endif
    }

    template<typename... ComponentPtrs>
    Entity* CreateEntity(ComponentPtrs... componentPtrs) {
#ifdef SHOWDEBUG
        std::cout << "Creating entity" << std::endl;
#endif
        Entity* entity = new Entity();
        (entity->AddComponent(componentPtrs), ...);
#ifdef SHOWDEBUG
        std::cout << "Entity created with " << sizeof...(ComponentPtrs) << " components." << std::endl;
        std::cout << std::endl;
#endif
        AddEntity(entity);
        return entity;
    }

    void AddEntity(Entity* entity) {
#ifdef SHOWDEBUG
        std::cout << "Adding entity " << entity << std::endl;
#endif
        for (auto& component : entity->GetComponents()) {
            std::type_index type = typeid(*component);
#ifdef SHOWDEBUG
            std::cout << "Adding component of type " << type.name() << std::endl;
#endif
            _entityComponents[type].insert(entity);
        }
        std::cout << std::endl;
    }

    template<typename T>
    std::vector<Entity*> GetEntitiesWithComponent() {
        std::vector<Entity*> entities;
        auto it = _entityComponents.find(typeid(T));
        if (it != _entityComponents.end()) {
            for (auto entity : it->second) {
                entities.push_back(entity);
            }
        }
        return entities;
    }

    template<typename T>
    Entity* GetFirstEntityWithComponent() {
        auto it = _entityComponents.find(typeid(T));
        if (it != _entityComponents.end()) {
            for (auto entity : it->second) {
                return entity;
            }
        }
        return nullptr;
    }

    void RemoveEntity(Entity* entity) {
        for (auto& component : entity->GetComponents()) {
            auto& entitySet = _entityComponents[typeid(*component)];
            auto it = entitySet.find(entity);
            if (it != entitySet.end())
            {
                entitySet.erase(it);
            }
        }
        delete entity;
    }
    
    void PrintEntitiesAndComponents() {
#ifdef SHOWDEBUG
        std::cout << "------- Printing entities -------" << std::endl;
        std::cout << "_entityComponents size: " << _entityComponents.size() << std::endl;
        if (_entityComponents.empty()) {
            std::cout << "No entities found" << std::endl;
            return;
        }
        for (auto& pair : _entityComponents) {
            std::cout << std::endl << "Components of type " << pair.first.name() << ":\n";
            for (auto entity : pair.second) {
                std::cout << "\tEntity " << entity << ":\n";
                for (auto component : entity->GetComponents()) {
                    std::cout << "\t\t" << typeid(*component).name() << "\n";
                }
            }
        }
        std::cout << "----------------------------------" << std::endl;
#endif
    }

    void PrintEntities() {
#ifdef SHOWDEBUG
        std::cout << "Entities:" << std::endl;
        std::unordered_set<Entity*> printedEntities;
        for (auto& pair : _entityComponents) {
            for (auto entity : pair.second) {
                if (printedEntities.find(entity) == printedEntities.end()) {
                    std::cout << entity << std::endl;
                    printedEntities.insert(entity);
                }
            }
        }
#endif
    }
};

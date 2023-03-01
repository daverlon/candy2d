#pragma once

#include "../stdafx.h"
#include "Entity.h"

// chatGPT!
class EntityManager {
private:
    std::unordered_map<std::type_index, std::unordered_set<Entity*>> _entityComponents;

public:
    EntityManager() {
        std::cout << "EntityManager()" << std::endl;
    }

    ~EntityManager() {
        std::cout << "~EntityManager()" << std::endl;
        for (auto& pair : _entityComponents) {
            for (auto entity : pair.second) {
                std::cout << "Deleting entity " << entity << "..." << std::endl;
                for (auto&c : entity->GetComponents()) {
                    std::cout << "    Deleting component " << c << " of type " << typeid(*c).name() << std::endl;
                    delete c;
                }
                delete entity;
            }
        }
        std::cout << "Finished deleting entity" << std::endl;
    }

    template<typename... ComponentPtrs>
    Entity* CreateEntity(ComponentPtrs... componentPtrs) {
        std::cout << "Creating entity" << std::endl;
        Entity* entity = new Entity();
        (entity->AddComponent(componentPtrs), ...);
        std::cout << "Entity created with " << sizeof...(ComponentPtrs) << " components." << std::endl;
        std::cout << std::endl;
        AddEntity(entity);
        return entity;
    }

    void AddEntity(Entity* entity) {
        std::cout << "Adding entity " << entity << std::endl;
        for (auto& component : entity->GetComponents()) {
            std::type_index type = typeid(*component);
            std::cout << "Adding component of type " << type.name() << std::endl;
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

    // void RemoveEntity(std::unique_ptr<Entity> entity) {
    //     for (auto& component : entity->GetComponents()) {
    //         std::type_index type = typeid(*component);
    //         auto& entitySet = _entityComponents[type];
    //         auto it = entitySet.find(std::unique_ptr<Entity>(entity.get()));
    //         if (it != entitySet.end()) {
    //             entitySet.erase(it);
    //         }
    //     }
    // }

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
    
    void PrintEntities() {
        std::cout << "------- Printing entities -------" << std::endl;
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
    }
};

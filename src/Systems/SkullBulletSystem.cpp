#include "SkullBulletSystem.h"

void SkullBulletSystem::Update(const float &dt) {

        for (auto& bullet : _entityManager->GetEntitiesWithComponent<SkullBulletComponent>()) {

        auto sc = bullet->GetComponent<SkullBulletComponent>();

        sc->TTL -= dt;
        if (sc->TTL <= 0.0f) bullet->FlagDeletion();
        // std::cout << Vec2toString(sc->GetDirection()) << ", " << sc->GetSpeed() << std::endl;;

        auto transform = bullet->GetComponent<TransformComponent>();

        transform->Translate(
            sc->GetSpeed() * sc->GetDirection() * dt
        );

    }
}

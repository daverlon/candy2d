#include "EnemyAISystem.h"

void EnemyAISystem::Update(const float &dt) {

    auto player = _entityManager->GetFirstEntityWithComponent<PlayerComponent>();
    auto playerTransform = player->GetComponent<TransformComponent>();


    for (auto& enemy : _entityManager->GetEntitiesWithComponent<EnemyAIComponent>()) {

        auto ms = enemy->GetComponent<EnemyAIComponent>()->GetMoveSpeed();

        auto transform = enemy->GetComponent<TransformComponent>();

        auto delta = playerTransform->GetPosition() - transform->GetPosition();

        auto movement = glm::normalize(delta) * dt * ms;

        transform->Translate(movement);

    }
}
#pragma once
#include <glm/glm.hpp>

#include "Component.h"
#include "EnemyComponent.h"
#include "GameObject.h"
#include "Transform.h"

namespace dae
{
	template <class T>
	class SpawnerComponent :
		public Component
	{
	public:
		SpawnerComponent(GameObject* pOwner, const glm::vec2& pos) : Component(pOwner)
		{
			const bool isBaseOf = std::is_base_of_v<EnemyComponent, T> == true;
			MG_ASSERT(isBaseOf);
			m_pEnemyComponent = pOwner->AddComponent<T>();
			m_pTransform = pOwner->AddComponent<Transform>();
			m_pTransform->SetWorldPosition(pos);
		}
		EnemyComponent* Spawn(GameObject* object) const
		{
			const auto enemyComponent = m_pEnemyComponent->Clone(object);
			return enemyComponent;
		}
	private:
		EnemyComponent* m_pEnemyComponent{ nullptr };
		Transform* m_pTransform{ nullptr };
	};
}


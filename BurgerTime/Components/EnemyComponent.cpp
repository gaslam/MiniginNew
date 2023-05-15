#include "EnemyComponent.h"

#include "GameObject.h"

dae::EnemyComponent::EnemyComponent(GameObject* object) : Component(object)
{
}

dae::EnemyComponent* dae::EnemyComponent::Clone(GameObject* object)
{
	if(object->IsComponentAdded<EnemyComponent>())
	{
		object->RemoveComponent<EnemyComponent>();
	}
	return object->AddComponent<EnemyComponent>();
}

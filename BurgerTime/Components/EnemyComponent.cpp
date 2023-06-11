#include "EnemyComponent.h"

#include <Misc/GameObject.h>

dae::EnemyComponent::EnemyComponent(GameObject* object) : Component(object)
{
}

void dae::EnemyComponent::Update(float)
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

void dae::EnemyComponent::CalculatePath(GameObject* target)
{
	auto pTargetTransform{ target->GetComponent<Transform>() };
	auto pTransform{ GetOwner()->GetComponent<Transform>() };
	MG_ASSERT_WARNING(pTargetTransform != nullptr,"Cannot calculate path!!")
	MG_ASSERT_WARNING(pTransform != nullptr,"Cannot calculate path!!")
	if(!pTransform && !pTargetTransform)
	{
		return;
	}

	Node* pStartNode{ m_pGridComponent->GetNodeAtPos(pTransform->GetWorldPosition()) };
	Node* pTargetNode{ m_pGridComponent->GetNodeAtPos(pTargetTransform->GetWorldPosition()) };
	MG_ASSERT_WARNING(pStartNode != nullptr, "Cannot calculate path!!")
	MG_ASSERT_WARNING(pTargetNode != nullptr, "Cannot calculate path!!")
	MG_ASSERT_WARNING(m_pGridComponent != nullptr, "Cannot calculate path!!")

	if(pStartNode && pTargetNode && m_pGridComponent)
	{
		m_Path = m_pGridComponent->AStar(pStartNode, pTargetNode);
	}
}

void dae::EnemyComponent::SetGrid(GridComponent* gridComponent)
{
	m_pGridComponent = gridComponent;
}

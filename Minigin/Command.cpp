#include "Command.h"
#include "GameObject.h"
#include <glm/fwd.hpp>
#include "HealthComponent.h"

void dae::MoveCommand::Execute(float deltaTime)
{
	Execute(m_Dir, deltaTime);
}

void dae::MoveCommand::Execute(glm::vec3& dir, float deltaTime)
{
	GameObject* pOwner = GetCommandOwner();
	if (!pOwner)
	{
		return;
	}
	Transform* pTransform = pOwner->GetComponent<Transform>();
	if (!pTransform)
	{
		return;
	}

	glm::vec3 pos = pTransform->GetLocalPosition();
	const float speed = pTransform->GetSpeedForMovement();
	dir *= speed * deltaTime;
	pos += dir;

	pTransform->SetLocalPosition(pos);
}

dae::GameObjectCommand::GameObjectCommand(GameObject* owner) : m_Owner{ owner }
{
}

void dae::DamageCommand::Execute(float)
{
	GameObject* pOwner = GetCommandOwner();
	if (pOwner)
	{
		return;
	}
	auto pHealth = pOwner->GetComponent<HealthComponent>();
	if (!pHealth)
	{
		return;
	}

	pHealth->Attack();
}

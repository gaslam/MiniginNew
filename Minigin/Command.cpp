#include "Command.h"
#include "GameObject.h"
#include <glm/fwd.hpp>

dae::Command::Command(GameObject* actor) :
	m_Owner{actor}
{
}

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
	pos.x += (dir.x * speed) * deltaTime;
	pos.y += (dir.y * speed) * deltaTime;
	pos.z += (dir.z * speed) * deltaTime;

	pTransform->SetLocalPosition(pos);
}

#include "Command.h"
#include "GameObject.h"
#include <glm/fwd.hpp>

dae::Command::Command(GameObject* actor) :
	m_Owner{actor}
{
}

void dae::MoveCommand::Execute()
{
	this->Execute(m_Dir.x, m_Dir.y);
}

void dae::MoveCommand::Execute(float x, float y)
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
	pos.x += x;
	pos.y += y;

	pTransform->SetLocalPosition(pos);
}

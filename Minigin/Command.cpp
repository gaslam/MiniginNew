#include "Command.h"
#include "GameObject.h"
#include <glm/fwd.hpp>
#include "CharacterComponent.h"

void dae::MoveCommand::Execute(float deltaTime)
{
	GameObject* pOwner = GetCommandOwner();
	if (!pOwner)
	{
		return;
	}
	Execute(m_Dir, deltaTime);

	CharacterComponent* pCharacter = pOwner->GetComponent<CharacterComponent>();
	if (!pCharacter)
	{
		return;
	}

	dae::CharacterComponent::CharacterState state{};
	if (m_Dir.x > 0)
	{
		state = dae::CharacterComponent::moveRight;
	}

	if (m_Dir.x < 0)
	{
		state = dae::CharacterComponent::moveLeft;
	}

	if (m_Dir.y > 0)
	{
		state = dae::CharacterComponent::moveUp;
	}

	if (m_Dir.y < 0)
	{
		state = dae::CharacterComponent::moveDown;
	}

	pCharacter->SetState(state);
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

void dae::SetChacterToStateIdleCommand::Execute(float)
{
	GameObject* pOwner = GetCommandOwner();
	if (!pOwner)
	{
		return;
	}

	CharacterComponent* pCharacter = pOwner->GetComponent<CharacterComponent>();
	if (!pCharacter)
	{
		return;
	}

	auto characterState = CharacterComponent::CharacterState::idle;

	pCharacter->SetState(characterState);
}

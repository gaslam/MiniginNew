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

	CharacterComponent::CharacterState characterState{pCharacter->GetState()};
	CharacterComponent::MovementState movementState{pCharacter->GetMovementState()};
	if (m_Dir.x > 0 && movementState == CharacterComponent::MovementState::LeftRight)
	{
		characterState = dae::CharacterComponent::moveRight;
	}

	if (m_Dir.x < 0 && movementState == CharacterComponent::MovementState::LeftRight)
	{
		characterState = dae::CharacterComponent::moveLeft;
	}

	if (m_Dir.y > 0 && movementState == CharacterComponent::MovementState::UpDown)
	{
		characterState = dae::CharacterComponent::moveUp;
	}

	if (m_Dir.y < 0 && movementState == CharacterComponent::MovementState::UpDown)
	{
		characterState = dae::CharacterComponent::moveDown;
	}

	pCharacter->SetState(characterState);
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
	CharacterComponent* pCharacter = pOwner->GetComponent<CharacterComponent>();
	if (pCharacter)
	{
		auto movementState = pCharacter->GetMovementState();
		pos.x += movementState == CharacterComponent::LeftRight ? (dir.x * speed) * deltaTime : 0.f;
		pos.y += movementState == CharacterComponent::UpDown ? (dir.y * speed) * deltaTime : 0.f;
	}
	else
	{
		pos.x += (dir.x * speed) * deltaTime;
		pos.y += (dir.y * speed) * deltaTime;
	}

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

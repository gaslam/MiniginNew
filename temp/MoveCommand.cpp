#include "MoveCommand.h"

#include "GameObject.h"
#include "Transform.h"
#include "Components/CharacterComponent.h"


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

	CharacterComponent::CharacterState characterState = pCharacter->GetState();
	const bool canMoveLeftRight = pCharacter->CanMoveLeftRight();
	const bool canMoveUpDown = pCharacter->CanMoveUpDown();
	bool isMoving = false;
	if (m_Dir.x > 0 && canMoveLeftRight)
	{
		characterState = dae::CharacterComponent::moveRight;
		isMoving = true;
	}

	if (m_Dir.x < 0 && !isMoving && canMoveLeftRight)
	{
		characterState = dae::CharacterComponent::moveLeft;
		isMoving = true;
	}

	if (m_Dir.y > 0 && !isMoving && canMoveUpDown)
	{
		characterState = dae::CharacterComponent::moveUp;
		isMoving = true;
	}

	if (m_Dir.y < 0 && !isMoving && canMoveUpDown)
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

	glm::vec2 pos = pTransform->GetLocalPosition();
	const float speed = pTransform->GetSpeedForMovement();
	CharacterComponent* pCharacter = pOwner->GetComponent<CharacterComponent>();
	if (pCharacter)
	{
		const bool moveX = pCharacter->CanMoveLeftRight();
		const bool moveY = pCharacter->CanMoveUpDown();
		pos.x += moveX ? (dir.x * speed) * deltaTime : 0.f;
		pos.y += moveY ? (dir.y * speed) * deltaTime : 0.f;
	}
	else
	{
		pos.x += (dir.x * speed) * deltaTime;
		pos.y += (dir.y * speed) * deltaTime;
	}

	pTransform->SetLocalPosition(pos);
}
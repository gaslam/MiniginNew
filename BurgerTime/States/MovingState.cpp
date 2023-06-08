#include "MovingState.h"
#include "IdleState.h"
#include "../Components/CharacterComponent.h"

void dae::MovingState::Update(CharacterComponent* pComponent, float deltaTime)
{
	pComponent->HandleMovement(m_Dir, deltaTime);
}

dae::CharacterState* dae::MovingState::HandleInput()
{
	InputManager& manager = InputManager::GetInstance();
	const bool isMovingLeft{ manager.GetInputKeyPressed("left") };
	const bool isMovingRight{ manager.GetInputKeyPressed("right") };
	const bool isMovingUp{ manager.GetInputKeyPressed("up") };
	const bool isMovingDown{ manager.GetInputKeyPressed("down") };
	const bool isNotMoving{ !isMovingLeft && !isMovingRight && !isMovingDown && !isMovingUp };
	if(isNotMoving)
	{
		return new IdleState{};
	}

	const bool isLeftKeyDown{ manager.GetInputKeyDown("left") };
	const bool isRightKeyDown{ manager.GetInputKeyDown("right") };
	const bool isUpKeyDown{ manager.GetInputKeyDown("up") };
	const bool isDownKeyDown{ manager.GetInputKeyDown("down") };

	if(isLeftKeyDown)
	{
		m_Dir.x = -1.f;
	}
	if (isRightKeyDown)
	{
		m_Dir.x = 1.f;
	}

	if(isUpKeyDown)
	{
		m_Dir.y = -1.f;
	}

	if (isDownKeyDown)
	{
		m_Dir.y = 1.f;
	}
	return nullptr;
}

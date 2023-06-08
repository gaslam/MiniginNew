#include "IdleState.h"

#include "MovingState.h"
#include "../Components/CharacterComponent.h"

dae::CharacterState* dae::IdleState::HandleInput()
{
	glm::vec2 dir{0, 0};
	const glm::vec2 oldDir{0, 0};
	if (InputManager::GetInstance().GetInputKeyDown("left"))
	{
		dir.x = -1;
	}
	if (InputManager::GetInstance().GetInputKeyDown("right"))
	{
		dir.x = 1;
	}
	if (InputManager::GetInstance().GetInputKeyDown("up"))
	{
		dir.y = -1;
	}
	if (InputManager::GetInstance().GetInputKeyDown("down"))
	{
		dir.y = 1;
	}
	if (dir == oldDir)
	{
		return nullptr;
	}
	MovingState* movingState{ new MovingState{ dir } };
	return movingState;
}

void dae::IdleState::OnEnter(CharacterComponent* pComponent)
{
	CharacterComponent::State state {CharacterComponent::State::idle};
	pComponent->SetState(state);
}

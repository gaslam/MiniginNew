#include "MoveCommand.h"

#include "GameObject.h"
#include "Transform.h"
#include "Components/CharacterComponent.h"


void dae::MoveCommand::Execute(float deltaTime)
{
	Execute(m_Dir, deltaTime);
}

void dae::MoveCommand::Execute(glm::vec2& dir, float deltaTime)
{
	GameObject* pOwner = GetCommandOwner();
	if (!pOwner)
	{
		return;
	}

	CharacterComponent* pCharacter = pOwner->GetComponent<CharacterComponent>();
	if (pCharacter)
	{
		pCharacter->HandleMovement(dir, deltaTime);
	}

}
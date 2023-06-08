#include "SetCharacterStateToIdleCommand.h"

#include "GameObject.h"
#include "Components/CharacterComponent.h"

void dae::SetCharacterStateToIdleCommand::Execute(float)
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
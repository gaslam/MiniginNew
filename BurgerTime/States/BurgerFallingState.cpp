#include "BurgerFallingState.h"
#include "../Components/BurgerComponent.h"

dae::BurgerState* dae::BurgerFallingState::HandleInput()
{
	return nullptr;
}

void dae::BurgerFallingState::OnEnter(BurgerComponent* pComponent)
{
	constexpr BurgerComponent::State state {BurgerComponent::State::falling};
	pComponent->SetState(state);
}

void dae::BurgerFallingState::Update(BurgerComponent* /*pComponent*/, float /*deltaTime*/)
{
}

#include "BurgerStandingState.h"
#include "../Components/CharacterComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/BurgerComponent.h"
#include "../CharacterManager.h"

#include "Logger.h"
#include <GameObject.h>

#include "BurgerTurningState.h"
#include "Shape.h"

dae::BurgerState* dae::BurgerStandingState::HandleInput()
{
	dae::GameObject* pPlayerObj{dae::CharacterManager::GetInstance().GetPlayer() };
	const auto pRigidBody{ pPlayerObj->GetComponent<dae::RigidBodyComponent>() };
	MG_ASSERT(pRigidBody != nullptr, "Cannot use RigidBody to collide with burger!!!")
		const auto pShape{ pRigidBody->GetShape() };
	dae::CharacterComponent* pPlayer{ pPlayerObj->GetComponent<dae::CharacterComponent>() };
	MG_ASSERT(pPlayer != nullptr, "Player has no character component!!")
	if(m_pShape->CollidesWith(pShape) && pPlayer->GetState() != CharacterComponent::idle)
	{
		return new BurgerTurningState{};
	}

	return nullptr;
}

void dae::BurgerStandingState::OnEnter(BurgerComponent* pComponent)
{
	m_pShape = pComponent->GetShape();
	constexpr BurgerComponent::State state {BurgerComponent::State::standingStill};
	pComponent->SetState(state);
}

#include "BurgerStandingState.h"
#include "../Components/CharacterComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/BurgerComponent.h"
#include "../Managers/CharacterManager.h"
#include "BurgerTurningState.h"
#include <Misc/Logger.h>
#include <Misc/GameObject.h>
#include <Misc/Shape.h>

#include "BurgerFallingState.h"

using namespace dae;

void BurgerStandingState::Update(BurgerComponent* pComponent, float)
{
	if(pComponent->GetState() == BurgerComponent::State::falling)
	{
		m_IsFalling = true;
	}
}

BurgerState* BurgerStandingState::HandleInput()
{
	GameObject* pPlayerObj{dae::CharacterManager::GetInstance().GetPlayer() };
	const auto pRigidBody{ pPlayerObj->GetComponent<dae::RigidBodyComponent>() };
	MG_ASSERT(pRigidBody != nullptr, "Cannot use RigidBody to collide with burger!!!")
		const auto pShape{ pRigidBody->GetShape() };
	dae::CharacterComponent* pPlayer{ pPlayerObj->GetComponent<dae::CharacterComponent>() };
	MG_ASSERT(pPlayer != nullptr, "Player has no character component!!")
		auto playerState{ pPlayer->GetState() };

	if(m_IsFalling)
	{
		return new BurgerFallingState{};
	}

	if(m_pShape->CollidesWith(pShape) && playerState != CharacterComponent::idle)
	{
		return new BurgerTurningState{};
	}

	return nullptr;
}

void BurgerStandingState::OnEnter(BurgerComponent* pComponent)
{
	m_pShape = pComponent->GetShape();
	constexpr BurgerComponent::State state {BurgerComponent::State::standingStill};
	pComponent->SetState(state);
}

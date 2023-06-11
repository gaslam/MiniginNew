#include "BurgerTurningState.h"

#include "BurgerFallingState.h"
#include "BurgerStandingState.h"
#include <Misc/GameObject.h>
#include "../Components/CharacterComponent.h"
#include "../Managers/CharacterManager.h"
#include "../Components/RigidBodyComponent.h"

#include <Misc/Logger.h>
#include "../Components/BurgerComponent.h"


using namespace dae;

BurgerState* BurgerTurningState::HandleInput()
{
	const GameObject* pPlayerObj{ CharacterManager::GetInstance().GetPlayer() };
	const auto pRigidBody{ pPlayerObj->GetComponent<RigidBodyComponent>() };
	MG_ASSERT(pRigidBody != nullptr, "Cannot use RigidBody to collide with burger!!!")
		const CharacterComponent* pPlayer {
		pPlayerObj->GetComponent<CharacterComponent>()
	};
	MG_ASSERT(pPlayer != nullptr, "Player has no character component!!")

		if (pPlayer->GetState() == CharacterComponent::idle || m_PlayerLeft)
		{
			return new BurgerStandingState{};
		}

	if (m_DegreesTurned >= m_TotalDegrees)
	{
		return new BurgerFallingState{};
	}

	return nullptr;
}

void BurgerTurningState::Update(BurgerComponent* pComponent, float deltaTime)
{
	m_DegreesTurned += static_cast<double>(deltaTime) * m_AngSpeed;
	pComponent->SetDegreesTurned(m_DegreesTurned);

	const GameObject* pPlayerObj{ CharacterManager::GetInstance().GetPlayer() };
	const auto pRigidBodyPlayer{ pPlayerObj->GetComponent<RigidBodyComponent>() };
	MG_ASSERT(pRigidBodyPlayer != nullptr, "Cannot use RigidBody to collide with burger!!!")
	const auto pBurgerShape{ pComponent->GetShape() };
	const auto pPlayerShape{ pRigidBodyPlayer->GetShape() };
	if(!pPlayerShape->CollidesWith(pBurgerShape))
	{
		m_PlayerLeft = true;
	}
}

void BurgerTurningState::OnEnter(BurgerComponent* pComponent)
{
	m_DegreesTurned = pComponent->GetDegreesTurned();
	constexpr BurgerComponent::State state {BurgerComponent::State::turning};
	pComponent->SetState(state);
}

#include "BurgerTurningState.h"

#include "BurgerFallingState.h"
#include "BurgerStandingState.h"
#include "GameObject.h"
#include "../Components/CharacterComponent.h"
#include "../CharacterManager.h"
#include "../Components/RigidBodyComponent.h"

#include "Logger.h"
#include "../Components/BurgerComponent.h"


dae::BurgerState* dae::BurgerTurningState::HandleInput()
{
	dae::GameObject* pPlayerObj{dae::CharacterManager::GetInstance().GetPlayer() };
	const auto pRigidBody{ pPlayerObj->GetComponent<dae::RigidBodyComponent>() };
	MG_ASSERT(pRigidBody != nullptr, "Cannot use RigidBody to collide with burger!!!")
	dae::CharacterComponent* pPlayer{ pPlayerObj->GetComponent<dae::CharacterComponent>() };
	MG_ASSERT(pPlayer != nullptr, "Player has no character component!!")

	if (pPlayer->GetState() == CharacterComponent::idle)
	{
		return new BurgerStandingState{};
	}

	if(m_DegreesTurned >= m_TotalDegrees)
	{
		return new BurgerFallingState{};
	}

	return nullptr;
}

void dae::BurgerTurningState::Update(BurgerComponent* pComponent, float deltaTime)
{
	m_DegreesTurned += static_cast<double>(deltaTime) * m_AngSpeed;
	pComponent->SetDegreesTurned(m_DegreesTurned);
}

void dae::BurgerTurningState::OnEnter(BurgerComponent* pComponent)
{
	m_DegreesTurned = pComponent->GetDegreesTurned();
	constexpr BurgerComponent::State state {BurgerComponent::State::turning};
	pComponent->SetState(state);
}

#include "BurgerFallingState.h"
#include "BurgerStandingState.h"
#include "../Components/BurgerComponent.h"
#include <Misc/Shape.h>

dae::BurgerState* dae::BurgerFallingState::HandleInput()
{
	if(!m_IsStateFalling)
	{
		return new BurgerStandingState{};
	}
	return nullptr;
}

void dae::BurgerFallingState::OnEnter(BurgerComponent* pComponent)
{
	constexpr BurgerComponent::State state {BurgerComponent::State::falling};
	pComponent->SetState(state);
	pComponent->SetDegreesTurned(0);
	m_StartPos = pComponent->GetPosition();
	auto pShape{ pComponent->GetShape() };
	const auto shapePoints{ pShape->GetPoints() };
	m_DistanceToTravel = static_cast<float>(shapePoints[1].y - shapePoints[0].y) * 2;
	pComponent->SetFallThroughGround(true);
}

void dae::BurgerFallingState::Update(BurgerComponent* pComponent, float /*deltaTime*/)
{
	auto newPos{ pComponent->GetPosition() };
	const float test{ newPos.y - m_StartPos.y };
	if(test >= m_DistanceToTravel)
	{
		pComponent->SetFallThroughGround(false);
	}
	m_IsStateFalling = pComponent->GetState() == BurgerComponent::State::falling;
}

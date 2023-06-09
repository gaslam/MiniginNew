#include "BurgerObserver.h"

#include "Event.h"
#include "GameObject.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/BurgerComponent.h"
#include "../States/BurgerFallingState.h"

using namespace dae;

void BurgerObserver::OnNotify(GameObject* object, Event& event)
{
	switch (event.GetId())
	{
	case EventType::FALLING:
		HandleFall(object);
		break;
	}
}

void BurgerObserver::HandleFall(GameObject* pGameObject)
{
	const auto pRigidBodyCompObject{ pGameObject->GetComponent<RigidBodyComponent>() };
	const auto pBurgerRigidBodyCompShape{ pRigidBodyCompObject->GetShape() };
	const auto pShape{ m_pBurgerComponent->GetShape() };

	if (!pShape->CollidesWith(pBurgerRigidBodyCompShape))
	{
		return;
	}
	auto pObjectBurgerComponent{ pGameObject->GetComponent<BurgerComponent>() };
	if (pObjectBurgerComponent && m_pBurgerComponent->GetState() == BurgerComponent::State::standingStill)
	{
		m_pBurgerComponent->SetState(new BurgerFallingState{});
	}
}

#include "PlatformObserver.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/BurgerComponent.h"
#include "../Components/RenderComponent.h"
#include <Misc/Event.h>
#include <Misc/GameObject.h>

using namespace dae;

void PlatformObserver::OnNotify(GameObject* object, Event& event)
{
	switch(event.GetId())
	{
	case EventType::HIT:
		HandleHit(object);
		break;
	case EventType::FALLING:
		HandleFall(object);
		break;
	}
}

void PlatformObserver::HandleHit(GameObject*)
{
}

void PlatformObserver::HandleFall(GameObject* pGameObject)
{
	const auto pRigidBodyCompObject{ pGameObject->GetComponent<RigidBodyComponent>() };
	const auto pBurgerRigidBodyCompShape{ pRigidBodyCompObject->GetShape() };
	const auto pShape{ m_pRigidBody->GetShape() };

	if (!pShape->CollidesWith(pBurgerRigidBodyCompShape))
	{
		return;
	}

	if(const auto pBurgerComponent{ pGameObject->GetComponent<BurgerComponent>() })
	{
		const auto pRenderComp{pGameObject->GetComponent<RenderComponent>()};
		const float height{ pRenderComp->GetFrameHeightScaled() };
		const float yPosPlaform{ static_cast<float>(pShape->GetPoints()[0].y) };
		const float yPosForDraw{ yPosPlaform - height };
		pBurgerComponent->StopFalling(yPosForDraw);
	}
}

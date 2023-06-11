#include "BurgerObserver.h"

#include <Misc/Event.h>
#include <Misc/GameObject.h>
#include "../Components/BurgerComponent.h"

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
	m_pBurgerComponent->HandleHitByObject(pGameObject);
}

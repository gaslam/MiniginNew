#include "PlatformObserver.h"
#include "../Components/PlatformComponent.h"
#include <Misc/Event.h>
#include <Misc/GameObject.h>

using namespace dae;

void PlatformObserver::OnNotify(GameObject* object, Event& event)
{
	switch(event.GetId())
	{
	case EventType::FALLING:
		HandleFall(object);
		break;
	}
}

void PlatformObserver::HandleFall(GameObject* pGameObject)
{
	m_pComponent->HandleHit(pGameObject);
}

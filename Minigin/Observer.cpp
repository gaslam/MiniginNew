#include "Observer.h"
#include "Event.h"
#include "GameObject.h"
#include "FPSComponent.h"
#include "TextComponent.h"
#include "HealthComponent.h"
#include "GameObject.h"

using namespace dae;
void PlayerDied::Notify(Event& event, GameObject* object)
{
	switch (event.GetId())
	{
	case EventType::PLAYER_DIED:
		DisplayGameOver(object);
		break;
	}
}

void PlayerDied::DisplayGameOver(GameObject* object)
{
	auto pTextComp = object->GetComponent<TextComponent>();
	if (pTextComp)
	{
		pTextComp->SetText("Game Over",true);
	}
}

void dae::PlayerDamaged::Notify(Event& event, GameObject* object)
{
	switch (event.GetId())
	{
	case EventType::PLAYER_DAMAGED:
		UpdateHealthDisplay(object);
		break;
	}
}

void dae::PlayerDamaged::UpdateHealthDisplay(GameObject* object)
{
	auto pHealthComponent = object->GetComponent<HealthComponent>();
	if (pHealthComponent)
	{
		
	}
}

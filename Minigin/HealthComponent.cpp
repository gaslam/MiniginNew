#include "HealthComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "ResourceManager.h"
#include "Event.h"

dae::HealthComponent::HealthComponent(GameObject* owner, int health) : Component(owner), m_Health{ health }
{
	const auto pFont{ dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 32) };
	auto fpsColorGreen = SDL_Color(0, 255, 0);
	const std::string text = "Lives " + std::to_string(m_Health);
	m_pTextComponent = owner->AddComponent<TextComponent>(text,pFont,fpsColorGreen);
	if (m_pTextComponent)
	{
		m_pTextComponent->Initialise();
		m_pTextComponent->SetCanRender(true);
	}
}

void dae::HealthComponent::Attack()
{
	auto pOwner = GetOwner();
	m_Health -= m_Damage;
	Event damagedEvent = Event(EventType::PLAYER_DAMAGED);
	pOwner->NotifyObservers(damagedEvent);
	if (m_Health <= 0)
	{
		Event deathEvent = Event(EventType::PLAYER_DIED);
		pOwner->NotifyObservers(deathEvent);
	}
}

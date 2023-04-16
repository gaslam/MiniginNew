#include "HealthComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "ResourceManager.h"
#include "Event.h"
#include <iostream>
#include "Subject.h"

dae::HealthComponent::HealthComponent(GameObject* owner, int health,SDL_Color color) : Component(owner), m_Health{ health }
{
	const auto pFont{ dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 32) };
	const std::string text = m_TextBegin + std::to_string(m_Health);
	m_pTextComponent = owner->AddComponent<TextComponent>(text, pFont, color);
	if (m_pTextComponent)
	{
		m_pTextComponent->Initialise();
		m_pTextComponent->SetCanRender(true);
	}
}

void dae::HealthComponent::Attack()
{
	if (m_Health <= 0 || m_IsDisabled)
	{
		return;
	}
	m_Health -= m_Damage;
	Event attackEvent = Event(EventType::PLAYER_DAMAGED);
	auto pOwner = GetOwner();
	if (m_Health <= 0)
	{
		attackEvent = Event(EventType::PLAYER_DIED);
	}
	Invoke(pOwner,attackEvent);
}

void dae::HealthComponent::OnHealthChanged(Event& event)
{
	auto pOwner = GetOwner();
	if (!pOwner)
	{
		return;
	}
	Invoke(pOwner, event);
}

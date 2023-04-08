#include "HealthComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "ResourceManager.h"
#include "Event.h"
#include <iostream>
#include "Delegate.h"
#include "ObserverManager.h"

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
	auto playerObserver = ObserverManager::GetInstance().AddObserver<PlayerObserver>();
	//m_Delegate.AddListener(std::bind(&PlayerObserver::OnNotify, playerObserver, std::placeholders::_2));
	m_Delegate.AddListener([playerObserver](Event& event, GameObject* object)
		{
			playerObserver->OnNotify(object, event); 
		});
}

void dae::HealthComponent::Attack()
{
	if (m_Health <= 0)
	{
		return;
	}
	m_Health -= m_Damage;
	Event damagedEvent = Event(EventType::PLAYER_DAMAGED);
	auto pOwner = GetOwner();
	m_Delegate.Invoke(damagedEvent, pOwner);
	if (m_Health <= 0)
	{
		Event deathEvent = Event(EventType::PLAYER_DIED);
	}
}

void dae::HealthComponent::OnHealthChanged(Event& event)
{
	auto pOwner = GetOwner();
	if (!pOwner)
	{
		return;
	}
	m_Delegate.Invoke(event, pOwner);
}

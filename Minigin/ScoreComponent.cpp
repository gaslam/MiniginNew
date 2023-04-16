#include "ScoreComponent.h"
#include "TextComponent.h"
#include "ResourceManager.h"
#include "Observer.h"
#include "Event.h"

dae::ScoreComponent::ScoreComponent(GameObject* owner, SDL_Color color) : Component(owner)
{
	const auto pFont{ dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 32) };
	auto colorGreen = color;
	const std::string text = m_TextBegin + std::to_string(m_Score);
	m_pTextComponent = owner->AddComponent<TextComponent>(text, pFont, colorGreen);
	if (m_pTextComponent)
	{
		m_pTextComponent->Initialise();
		m_pTextComponent->SetCanRender(true);
	}
}

void dae::ScoreComponent::AddScore(int score)
{
	m_Score += score;
	OnScoreChanged();
}

void dae::ScoreComponent::SetLocalPos(glm::vec2& pos)
{
	m_pTextComponent->SetPosition(pos);
}

void dae::ScoreComponent::OnScoreChanged()
{
	auto pOwner = GetOwner();
	Event event = Event(EventType::SCORE_VALUE_CHANGED);
	Invoke(pOwner,event);
}

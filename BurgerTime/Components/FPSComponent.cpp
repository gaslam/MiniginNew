#include "FPSComponent.h"
#include "TextComponent.h"
#include "GameObject.h"

void dae::FPSComponent::Update(float deltaTime)
{
	m_AccumulatedTime += deltaTime;
	++m_CountFPS;
	if (m_AccumulatedTime > sec)
	{
		m_AccumulatedTime -= sec;
		m_FPSValue = std::to_string(m_CountFPS);
		m_CountFPS = 0;
		m_NeedsUpdate = true;
	}

	if (m_TextRenderComp)
	{
		if (m_NeedsUpdate)
		{
			m_TextRenderComp->SetText(m_FPSValue);
			m_NeedsUpdate = false;
		}
		m_TextRenderComp->Update(deltaTime);
	}
}

dae::FPSComponent::FPSComponent(GameObject* object) : Component{object}, m_NeedsUpdate{ true }
{
}

void dae::FPSComponent::Initialise()
{
	auto owner = GetOwner();
	MG_ASSERT(owner != nullptr, "Cannot get owner!!");
	if (!owner)
	{
		return;
	}
	m_TextRenderComp = owner->GetComponent<dae::TextComponent>();
	MG_ASSERT(m_TextRenderComp != nullptr,"Cannot add TextRenderComp!!");
}

void dae::FPSComponent::Render() const
{
	if (m_TextRenderComp != nullptr)
	{
		m_TextRenderComp->Render();

	}
}

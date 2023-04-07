#include "FPSComponent.h"
#include <stdexcept>
#include <SDL_ttf.h>
#include <glm/glm.hpp>
#include "Renderer.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "Texture2D.h"

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
			m_TextRenderComp->SetText(m_FPSValue,true);
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
	m_TextRenderComp = owner->GetComponent<dae::TextComponent>();
}

void dae::FPSComponent::Render() const
{
	if (m_TextRenderComp != nullptr)
	{
		m_TextRenderComp->Render();

	}
}

#include <stdexcept>
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "RenderComponent.h"

dae::TextComponent::TextComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font, SDL_Color& color)
	: Component(owner), m_NeedsUpdate(true), m_text(text), m_font(std::move(font)), m_Color{ color }
	{ }

	void dae::TextComponent::Initialise()
	{
		auto owner = GetOwner();
		if (owner)
		{
			m_RenderComponent = owner->AddComponent<dae::RenderComponent>(true);
			m_RenderComponent->SetPosition(50.f, 50.f);
		}
	}

	void dae::TextComponent::Update(float)
	{
		if (m_NeedsUpdate)
		{
			const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_Color);
			if (surf == nullptr)
			{
				throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
			}
			auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
			if (texture == nullptr)
			{
				throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
			}
			SDL_FreeSurface(surf);

			if (m_RenderComponent)
			{
				m_RenderComponent->SetTexture(std::make_shared<Texture2D>(texture));
			}

			m_NeedsUpdate = false;
		}

	}

	void dae::TextComponent::Render() const
	{
		m_RenderComponent->Render();
	}

	// This implementation uses the "dirty flag" pattern
	void dae::TextComponent::SetText(const std::string& text, bool display)
	{
		m_text = text;
		SetCanRender(display);
	}

	void dae::TextComponent::SetPosition(const float x, const float y)
	{
		m_RenderComponent->SetPosition(x, y);
	}

	void dae::TextComponent::SetCanRender(bool display)
	{
		if (display)
		{
			m_NeedsUpdate = true;
		}
		m_RenderComponent->SetCanRender(display);
	}



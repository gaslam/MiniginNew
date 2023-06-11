#pragma once
#include <string>
#include <memory>
#include <Misc/Component.h>
#include <SDL.h>

namespace dae
{
	class Font;
	class RenderComponent;
	class TextComponent final : public Component
	{
	public:
		void Initialise();
		void Update(float deltaTime) override;
		void Render() const override {};

		void SetText(const std::string& text);
		void SetPosition(float x, float y);

		TextComponent(GameObject* owner,const std::string& text, std::shared_ptr<Font> font, SDL_Color& color);
		~TextComponent() override = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;
	private:
		bool m_NeedsUpdate{};
		std::string m_text{};
		std::shared_ptr<Font> m_font{};
		RenderComponent* m_RenderComponent{};
		SDL_Color m_Color{};
	};
}

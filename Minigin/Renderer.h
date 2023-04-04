#pragma once
#include <SDL.h>
#include "Singleton.h"
#include <memory>
#include <glm/glm.hpp>


namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};
		//std::unique_ptr<ImGuiTempRenderer> m_TempRenderer;
		bool m_ShowDemo{ false };
	public:
		void Init(SDL_Window* window);
		void Render();
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
		glm::ivec2 GetWindowWidthAndHeight() const {
			glm::ivec2 widthAndHeight{};
			SDL_GetWindowSize(m_window, &widthAndHeight.x, &widthAndHeight.y);
			return widthAndHeight;
		}
	};
}


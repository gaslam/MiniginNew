#include "GridComponent.h"

#include <SDL_render.h>

#include "Renderer.h"

void dae::GridComponent::RenderImGUI()
{
	ImGui::Text("Grid:");
	ImGui::NewLine();
	ImGui::Checkbox("Draw grid", &m_DrawGrid);
}
void dae::GridComponent::RenderGrid() const
{
	if (m_DrawGrid)
	{
		auto renderer = dae::Renderer::GetInstance().GetSDLRenderer();
		SDL_SetRenderDrawColor(renderer, Uint8{ 255 }, Uint8{ 0 }, Uint8{ 0 }, Uint8{ 255 });
		const int startPosX{ static_cast<int>(m_StartPos.x) };
		const int startPosY{ static_cast<int>(m_StartPos.y) };
		for (int i{ startPosX + m_XStep }; i < m_MaxX + startPosX; i += m_XStep)
		{
			SDL_RenderDrawLine(renderer, i,startPosY,i,m_MaxY + startPosY);
		}
		for (int i{ startPosY + m_YStep }; i < m_MaxY + startPosY; i += m_YStep)
		{
			SDL_RenderDrawLine(renderer, startPosX, i, m_MaxX + startPosX, i);
		}

	}
}

void dae::GridComponent::Render() const
{
	RenderGrid();
}

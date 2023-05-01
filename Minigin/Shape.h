#pragma once
#include <SDL_rect.h>
#include <vector>
#include <glm/glm.hpp>
#include "Renderer.h"

class Shape
{
public:
    virtual bool CollidesWith(const Shape& other) const = 0;
    virtual std::vector<glm::ivec2> GetPoints() const = 0;
    virtual void Render() const {}
    virtual void SetPosition(glm::ivec2& pos) = 0;
    virtual ~Shape() = default;
};

class RectangleShape: public Shape
{
public:
    explicit RectangleShape(const SDL_Rect& rect) : m_Rect{ rect } {}
	RectangleShape(glm::ivec2& startPos, int width, int height)
    {
        m_Rect = SDL_Rect{ startPos.x,startPos.y, width, height };
    }

    bool CollidesWith(const Shape& other) const override
    {
        if (const auto* otherRect = dynamic_cast<const RectangleShape*>(&other)) {
            return SDL_HasIntersection(&m_Rect, &otherRect->m_Rect);
        }
        //More shapes possible
        return false;
    }

    void SetPosition(glm::ivec2& pos) override
    {
        m_Rect.x = pos.x;
        m_Rect.y = pos.y;
    }

    void Render() const override
    {
        auto renderer = dae::Renderer::GetInstance().GetSDLRenderer();
        SDL_SetRenderDrawColor(renderer, Uint8{ 255 }, Uint8{ 0 }, Uint8{ 0 }, Uint8{ 255 });
        SDL_RenderDrawRect(renderer, &m_Rect);
    }

	std::vector<glm::ivec2> GetPoints() const override
    {
        std::vector<glm::ivec2> points{};
        points.emplace_back(m_Rect.x, m_Rect.y);
        points.emplace_back(m_Rect.x, m_Rect.y + m_Rect.h);
        points.emplace_back(m_Rect.x + m_Rect.w, m_Rect.y + m_Rect.h);
        points.emplace_back(m_Rect.x, m_Rect.y + m_Rect.h);
        return points;
    }
private:
    SDL_Rect m_Rect{};
};




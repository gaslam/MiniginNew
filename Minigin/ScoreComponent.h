#pragma once
#include "Component.h"
#include "Delegate.h"
#include <string>
#include "SDL_ttf.h"
class Event;
namespace dae {
	class TextComponent;
	class ScoreComponent : public Component
	{
	public:
		ScoreComponent(GameObject* owner, SDL_Color color = SDL_Color{255,255,255});
		void Update(float) override {};
		void Render() const override { }
		void AddScore(int score);
		void SetLocalPos(glm::vec2& pos);
		[[nodiscard]] int GetScore()const { return m_Score; }
		[[nodiscard]] std::string GetTextBegin() const { return m_TextBegin; }
	private:
		void OnScoreChanged();
		int m_Score{};
		const std::string m_TextBegin{ "Score " };
		TextComponent* m_pTextComponent{ nullptr };
		Delegate<Event&, GameObject*> m_Delegate{};
	};
}


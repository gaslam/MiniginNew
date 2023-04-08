#pragma once
#include "Component.h"
#include <string>
#include <functional>
#include "Delegate.h"
#include "SDL_ttf.h"
class Event;
namespace dae {
	class TextComponent;
	class HealthComponent :
		public Component
	{
	public:
		HealthComponent(GameObject* owner, int health, SDL_Color color = {255,255,255});
		int GetHealth() const { return m_Health; };
		int SetDamage(int damage) { m_Damage = damage; }
		void Attack();
		void Update(float) override {};
		void Render() const override { }
		void OnHealthChanged(Event& event);
		[[nodiscard]] std::string GetTextBegin() const { return m_TextBegin; }
	private:
		int m_Health{};
		int m_Damage{ 1 };
		const std::string m_TextBegin{ "Lives: " };
		TextComponent* m_pTextComponent{nullptr};
		Delegate<Event&,GameObject*> m_Delegate{};
	};
}


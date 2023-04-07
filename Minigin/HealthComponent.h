#pragma once
#include "Component.h"
namespace dae {
	class TextComponent;
	class HealthComponent :
		public Component
	{
	public:
		HealthComponent(GameObject* owner, int health);
		int GetHealth() const { return m_Health; };
		int SetDamage(int damage) { m_Damage = damage; }
		void Attack();
		void Update(float) override {};
		void Render() const override { }
	private:
		int m_Health{};
		int m_Damage{ 1 };
		TextComponent* m_pTextComponent{nullptr};
	};
}


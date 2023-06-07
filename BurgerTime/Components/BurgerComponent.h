#pragma once
#include <Component.h>

namespace dae
{
	class RenderComponent;
	class BurgerState;
	class RectangleShape;
	class BurgerComponent :
    public Component
{
	public:
		BurgerComponent(GameObject* pObject, const std::string& file);
		void Update(float) override;
		RectangleShape* GetShape() const { return m_pShape; }
		enum class State
		{
			standingStill,
			turning,
			falling
		};
		void SetState(State state) { m_State = state; }
		void SetDegreesTurned(double degrees);
		double GetDegreesTurned() const { return m_DegreesTurned; }
	private:
		double m_DegreesTurned{};

		State m_State{};

		RenderComponent* m_pRenderComp{};
		RectangleShape* m_pShape{};
		std::unique_ptr<BurgerState> m_pBurgerState{};
		void HandleInput();
};
}



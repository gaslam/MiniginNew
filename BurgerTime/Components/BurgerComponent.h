#pragma once
#include <Component.h>

#include <Subject.h>
#include <glm/vec2.hpp>

namespace dae
{
	class RenderComponent;
	class BurgerState;
	class RectangleShape;
	class BurgerComponent :
    public Component, public Subject
{
	public:
		BurgerComponent(GameObject* pObject, const std::string& file, const glm::vec2& pos, float scale);
		void Update(float) override;
		RectangleShape* GetShape() const { return m_pShape; }
		enum class State
		{
			standingStill,
			turning,
			falling
		};
		void SetState(State state);
		void SetState(BurgerState* state);
		void SetDegreesTurned(double degrees);
		void StopFalling(const float hitYPos);
		void SetFallThroughGround(bool isAllowed) const;
		State GetState() const { return m_State; }
		double GetDegreesTurned() const { return m_DegreesTurned; }
		glm::vec2 GetPosition() const;
	private:
		double m_DegreesTurned{};

		State m_State{};

		RenderComponent* m_pRenderComp{};
		RectangleShape* m_pShape{};
		std::unique_ptr<BurgerState> m_pBurgerState{};
		void HandleInput();
		void HandleFall() const;
};
}



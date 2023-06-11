#pragma once
#include <Misc/Component.h>

#include <Misc/Subject.h>
#include <glm/vec2.hpp>

namespace dae
{
	class AudioComponent;
	class RenderComponent;
	class BurgerState;
	class RectangleShape;
	class BurgerComponent :
    public Component, public Subject
{
	public:
		BurgerComponent(GameObject* pObject, const std::string& file, const glm::vec2& pos, float scale);
		void Start() override;
		void Update(float) override;
		RectangleShape* GetShape() const { return m_pShape; }
		enum class State
		{
			standingStill,
			turning,
			falling
		};
		void SetState(State state);
		void SetDegreesTurned(double degrees);
		void StopFalling(const float hitYPos);
		void SetFallThroughGround(bool isAllowed) const;
		State GetState() const { return m_State; }
		double GetDegreesTurned() const { return m_DegreesTurned; }
		glm::vec2 GetPosition() const;
		void HandleHitByObject(GameObject* pFallingObject);
		void PlayBounceSound(bool once);
		void StopBounceSound();

	private:
		double m_DegreesTurned{};

		glm::vec2 m_OriginalPosition{};
		State m_State{};

		RenderComponent* m_pRenderComp{};
		RectangleShape* m_pShape{};
		std::unique_ptr<BurgerState> m_pBurgerState{};
		void HandleInput();
		void HandleFall() const;

		AudioComponent* m_pBurgerSound{};
};
}



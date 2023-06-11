#pragma once
#include <Misc/Component.h>
#include <map>
#include <glm/glm.hpp>
#include "../States/PlayerState.h"
namespace dae {

	struct AnimationItem {
		bool isRepeatable;
		bool isXflipped;
		bool isYflipped;
		int startRow;
		int startCol;
		int count;
		int holdFrame;
	};
	class AnimationComponent;
	class CharacterComponent : public Component
	{
	public:
		CharacterComponent(GameObject* pOwner, AnimationComponent* pComponent);
		enum State : int {
			idle,
			moveLeft,
			moveRight,
			moveUp,
			moveDown,
			attack,
			trowingPepper
		};
		void Update(float) override;
		void Render() const override {};
		void RenderImGUI() override;
		void AddAnimation(AnimationItem& animation, State& state);
		void SetAnimation(State& state);
		void SetAnimation(int id);
		void HandleMovement(glm::vec2& dir, float elapsedTime);
		State GetState() const { return m_State; }
		bool CanMoveUpDown() const { return m_CanMoveUpDown; }
		bool CanMoveLeftRight() const { return m_CanMoveLeftRight; }
		void SetState(State& state);
		void SetMovementUpDown(bool canMove);
		void SetMovementLeftRight(bool canMove);
		void Reset(glm::vec2 pos);
	private:
		int m_Lives{4};
		std::unique_ptr<CharacterState> m_CharacterState;
		State m_State;
		AnimationComponent* m_pAnimationComponent;
		bool m_CanMoveUpDown{ false };
		bool m_CanSetMoveLeftRight{ true };
		bool m_CanSetMoveUpDown{ true };
		bool m_CanMoveLeftRight{ true };
		bool m_IsMoving{ false };
		
		std::map< State, AnimationItem> m_Animations{};
		void HandleInput();
	};
}


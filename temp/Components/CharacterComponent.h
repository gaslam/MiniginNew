#pragma once
#include <Component.h>
#include <map>

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
		CharacterComponent(GameObject* pOwner, AnimationComponent* pComponent) : Component(pOwner), m_pAnimationComponent{ pComponent }, m_CharacterState{} {};
		void Update(float) override;
		void Render() const override {};
		enum CharacterState : int {
			idle,
			moveLeft,
			moveRight,
			moveUp,
			moveDown,
			attack,
			trowingPepper
		};
		void AddAnimation(AnimationItem& animation, CharacterState& state);
		void SetAnimation(CharacterState& state);
		void SetAnimation(int id);
		CharacterState GetState() const { return m_CharacterState; }
		bool CanMoveUpDown() const { return m_CanMoveUpDown; }
		bool CanMoveLeftRight() const { return m_CanMoveLeftRight; }
		void SetState(CharacterState& state);
		void SetMovementUpDown(bool canMove);
		void SetMovementLeftRight(bool canMove);
	private:
		CharacterState m_CharacterState;
		AnimationComponent* m_pAnimationComponent;
		bool m_CanMoveUpDown{ false };
		bool m_CanSetMoveLeftRight{ true };
		bool m_CanSetMoveUpDown{ true };
		bool m_CanMoveLeftRight{ true };
		std::map< CharacterState, AnimationItem> m_Animations{};
	};
}


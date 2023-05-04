#pragma once
#include "Component.h"
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
		void Update(float) override {};
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
		enum MovementState
		{
			LeftRight,
			UpDown
		};
		void AddAnimation(AnimationItem& animation, CharacterState& state);
		void SetAnimation(CharacterState& state);
		void SetAnimation(int id);
		CharacterState GetState() const { return m_CharacterState; }
		MovementState GetMovementState() const { return m_MovementState; }
		void SetState(CharacterState& state);
	private:
		CharacterState m_CharacterState;
		MovementState m_MovementState{};
		AnimationComponent* m_pAnimationComponent;
		std::map< CharacterState, AnimationItem> m_Animations{};
	};
}


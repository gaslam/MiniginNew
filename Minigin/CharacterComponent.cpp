
#include "CharacterComponent.h"
#include "AnimationComponent.h"
#include <algorithm>

void dae::CharacterComponent::AddAnimation(AnimationItem& animation, CharacterState& state)
{
	MG_ASSERT(animation.count > -1);
	MG_ASSERT(animation.startCol > -1);
	MG_ASSERT(animation.startRow > -1);
	m_Animations.emplace(state, animation);
}

void dae::CharacterComponent::SetAnimation(CharacterState& state)
{
	const bool isStateInConflict1{ state == moveUp && m_MovementState == LeftRight };
	const bool isStateInConflict2{ state == moveLeft && m_MovementState == UpDown || state == moveRight && m_MovementState == UpDown };
	if (isStateInConflict1 || isStateInConflict2)
	{
		return;
	}

	auto it = std::find_if(m_Animations.begin(), m_Animations.end(), [state](std::pair<CharacterState, dae::AnimationItem> pair) {
		return pair.first == state;
		});
	if (it == m_Animations.end())
	{
		return;
	}

	auto pair = *it;
	AnimationItem item = pair.second;
	if (m_pAnimationComponent)
	{
		m_CharacterState = state;
		m_pAnimationComponent->ChangeAnimation(item.startRow,item.startCol,item.count,item.isRepeatable,item.isXflipped,item.isYflipped);
	}
}

void dae::CharacterComponent::SetAnimation(int id)
{
	CharacterState state{ id };
	SetAnimation(state);
}

void dae::CharacterComponent::SetState(CharacterState& state)
{
	if (m_CharacterState == state)
	{
		return;
	}
	SetAnimation(state);
}


#include "CharacterComponent.h"
#include "AnimationComponent.h"
#include <algorithm>
#include <windows.h>

void dae::CharacterComponent::AddAnimation(AnimationItem& animation, CharacterState& state)
{
	MG_ASSERT(animation.count > -1);
	MG_ASSERT(animation.startCol > -1);
	MG_ASSERT(animation.startRow > -1);
	m_Animations.emplace(state, animation);
}

void dae::CharacterComponent::SetAnimation(CharacterState& state)
{
	auto it = std::find_if(m_Animations.begin(), m_Animations.end(), [state](std::pair<dae::CharacterComponent::CharacterState, dae::AnimationItem> pair) {
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
	if (m_State == state)
	{
		return;
	}
	m_State = state;
	SetAnimation(state);
}

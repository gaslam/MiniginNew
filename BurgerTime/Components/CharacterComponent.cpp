
#include "CharacterComponent.h"
#include "AnimationComponent.h"
#include <algorithm>
#include <imgui.h>

#include <Misc/GameObject.h>
#include <Audio/Audio.h>
#include "../States/IdleState.h"

dae::CharacterComponent::CharacterComponent(GameObject* pOwner, AnimationComponent* pComponent) : Component(pOwner),
m_CharacterState{ std::make_unique<IdleState>() }, m_pAnimationComponent{ pComponent }, m_State{}
{
}

void dae::CharacterComponent::Update(float deltaTime)
{
	HandleInput();
	m_CharacterState->Update(this, deltaTime);
	m_CanSetMoveLeftRight = true;
	m_CanSetMoveUpDown = true;
}

void dae::CharacterComponent::RenderImGUI()
{
	if (ImGui::CollapsingHeader("Player"))
	{
		ImGui::Text("Player:");
		ImGui::NewLine();
		ImGui::Text("Movement:");
		ImGui::NewLine();
		ImGui::Text("WASD (QWERTY)");
		ImGui::Text("Controller DPAD");
		ImGui::NewLine();
		ImGui::Text("Movement sound not in game");
		ImGui::Text("See states folder for states");
		ImGui::NewLine();
		auto pTransform = GetOwner()->GetComponent<Transform>();
		const auto position = pTransform->GetLocalPosition();
		ImGui::NewLine();
		ImGui::Text("Position:");
		ImGui::Text("x = %.2f", position.x);
		ImGui::Text("y = %.2f", position.y);
	}

}

void dae::CharacterComponent::AddAnimation(AnimationItem& animation, State& state)
{
	MG_ASSERT(animation.count > -1, "Animation count must be 0 or higher!!");
	MG_ASSERT(animation.startCol > -1, "The column where the animation starts must have an index of 0 or higher!!");
	MG_ASSERT(animation.startRow > -1, "The row where the animation starts must have an index of 0 or higher!!");
	m_Animations.emplace(state, animation);
}

void dae::CharacterComponent::SetAnimation(State& state)
{
	auto it = std::find_if(m_Animations.begin(), m_Animations.end(), [state](std::pair<State, dae::AnimationItem> pair) {
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
		m_State = state;
		m_pAnimationComponent->ChangeAnimation(item.startRow, item.startCol, item.count, item.isRepeatable, item.isXflipped, item.isYflipped);
	}
}

void dae::CharacterComponent::SetAnimation(int id)
{
	State state{ id };
	SetAnimation(state);
}

void dae::CharacterComponent::HandleMovement(glm::vec2& dir, float deltaTime)
{
	GameObject* pOwner{ GetOwner() };

	MG_ASSERT(pOwner != nullptr, "Owner cannot be null!!");

	Transform* pTransform = pOwner->GetComponent<Transform>();
	if (!pTransform)
	{
		return;
	}

	glm::vec2 pos = pTransform->GetLocalPosition();
	const float speed = pTransform->GetSpeedForMovement();

	pos.x += m_CanMoveLeftRight ? (dir.x * speed) * deltaTime : 0.f;
	pos.y += m_CanMoveUpDown ? (dir.y * speed) * deltaTime : 0.f;
	pTransform->SetLocalPosition(pos);
	CharacterComponent::State state {CharacterComponent::State::idle};
	if (dir.x < 0)
	{
		state = CharacterComponent::State::moveLeft;
	}
	if (dir.x > 0)
	{
		state = CharacterComponent::State::moveRight;
	}

	if (dir.y > 0)
	{
		state = CharacterComponent::State::moveUp;
	}
	if (dir.y < 0)
	{
		state = CharacterComponent::State::moveDown;
	}
	SetState(state);
}

void dae::CharacterComponent::SetState(State& state)
{
	if (m_State == state)
	{
		return;
	}
	SetAnimation(state);
}

void dae::CharacterComponent::SetMovementLeftRight(bool canMove)
{
	if (m_CanSetMoveLeftRight)
	{
		m_CanMoveLeftRight = canMove;
	}
	if (canMove)
	{
		m_CanSetMoveLeftRight = false;
	}
}

void dae::CharacterComponent::HandleInput()
{
	const auto newState = m_CharacterState->HandleInput();
	if (newState != nullptr)
	{
		m_CharacterState->OnExit(this);
		m_CharacterState = std::unique_ptr<CharacterState>(newState);
		m_CharacterState->OnEnter(this);
	}
}

void dae::CharacterComponent::SetMovementUpDown(bool canMove)
{
	if (m_CanSetMoveUpDown)
	{
		m_CanMoveUpDown = canMove;
	}
	if (canMove)
	{
		m_CanSetMoveUpDown = false;
	}
}

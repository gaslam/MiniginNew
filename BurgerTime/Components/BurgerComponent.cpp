#include "BurgerComponent.h"

#include <Misc/Event.h>
#include <Misc/GameObject.h>
#include <Misc/Shape.h>
#include "../Components/CharacterComponent.h"
#include "../Components/RenderComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../States/BurgerState.h"
#include "../States/BurgerStandingState.h"

using namespace dae;

BurgerComponent::BurgerComponent(GameObject* pObject, const std::string& file, const glm::vec2& pos, float scale) : Component{ pObject }, m_pBurgerState{ std::make_unique<BurgerStandingState>() }
,m_OriginalPosition{pos}
{
	auto pOwner{ GetOwner() };
	m_pRenderComp = pOwner->GetComponent<RenderComponent>();
	if (!m_pRenderComp)
	{
		m_pRenderComp = pOwner->AddComponent<RenderComponent>(file);
	}
	m_pRenderComp->SetScale(scale);
	Transform* pTransform{ pOwner->GetComponent<Transform>() };
	if (!pTransform)
	{
		pTransform = pOwner->AddComponent<Transform>();
	}
	pTransform->SetWorldPosition(pos);
	const int width{ static_cast<int>(m_pRenderComp->GetFrameWidthScaled()) };
	const int height{ static_cast<int>(m_pRenderComp->GetFrameHeightScaled()) };
	m_pShape = new RectangleShape{ pos, width,height };
	const auto pRigidBody{ pOwner->AddComponent<RigidBodyComponent>(pTransform) };
	pRigidBody->SetShape(m_pShape);
}

void BurgerComponent::Start()
{
	auto pOwner{ GetOwner() };
	Transform* pTransform{ pOwner->GetComponent<Transform>() };
	if (!pTransform)
	{
		pTransform = pOwner->AddComponent<Transform>();
	}
	pTransform->SetWorldPosition(m_OriginalPosition);
	SetState(new BurgerStandingState{});
	m_DegreesTurned = 0;
	m_pRenderComp->SetAngle(m_DegreesTurned);
}

void BurgerComponent::Update(float deltaTime)
{
	HandleInput();
	m_pBurgerState->Update(this, deltaTime);

	if (m_State == State::falling)
	{
		GameObject* pOwner{ GetOwner() };
		Event event{ EventType::FALLING };
		Invoke(pOwner, event);
	}

}

void BurgerComponent::SetState(State state)
{
	m_State = state;
	if (m_State == State::falling)
	{
		HandleFall();
	}
}

void BurgerComponent::HandleFall() const
{
	const auto pOwner{ GetOwner() };
	const auto pRigidBody{ pOwner->GetComponent<RigidBodyComponent>() };

	MG_ASSERT_WARNING(pRigidBody, "Cannot let object fall!! Rigidbody missing")
		if (pRigidBody)
		{
			constexpr bool isStatic{ false };
			pRigidBody->SetIsStatic(isStatic);
		}
}

void BurgerComponent::SetDegreesTurned(double degrees)
{
	m_DegreesTurned = degrees;
	m_pRenderComp->SetAngle(m_DegreesTurned);
}

void BurgerComponent::StopFalling(const float hitYPos)
{
	GameObject* pOwner{ GetOwner() };
	if (const auto pRigidBody{ pOwner->GetComponent<RigidBodyComponent>() })
	{
		if(pRigidBody->IsAllowedThroughGround())
		{
			return;
		}
		constexpr bool isStatic{ true };
		pRigidBody->SetIsStatic(isStatic);
	}

	auto pTransform{ pOwner->GetComponent<Transform>() };
	auto localPos{ pTransform->GetWorldPosition() };
	localPos.y = hitYPos;
	pTransform->SetLocalPosition(localPos);
	Event event{ EventType::HIT };
	Invoke(pOwner, event);
	SetState(State::standingStill);
}

void BurgerComponent::SetFallThroughGround(bool isAllowed) const
{
	const auto pOwner{ GetOwner() };
	const auto pRigidBody{ pOwner->GetComponent<RigidBodyComponent>() };
	MG_ASSERT_WARNING(pRigidBody != nullptr, "Cannot get BurgerComponent RigidBody!!");
	if (pRigidBody)
	{
		pRigidBody->SetAllowThroughGround(isAllowed);
	}
}

glm::vec2 BurgerComponent::GetPosition() const
{
	const auto pOwner{ GetOwner() };
	glm::vec2 pos{};
	const auto pTransform{ pOwner->GetComponent<Transform>() };
	MG_ASSERT_WARNING(pTransform != nullptr, "Cannot get BurgerComponent transform!!");
	if(pTransform)
	{
		pos = pTransform->GetWorldPosition();
	}
	return pos;
}

void BurgerComponent::HandleInput()
{
	if (const auto newState = m_pBurgerState->HandleInput())
	{
		SetState(newState);
	}
}

void BurgerComponent::SetState(BurgerState* state)
{

	m_pBurgerState->OnExit(this);
	m_pBurgerState = std::unique_ptr<BurgerState>(state);
	m_pBurgerState->OnEnter(this);
}

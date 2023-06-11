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

BurgerComponent::BurgerComponent(GameObject* pObject, const std::string& file, const glm::vec2& pos, float scale) : Component{ pObject }
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
	SetState(State::standingStill);
	m_pBurgerState = std::make_unique<BurgerStandingState>();
	m_pBurgerState->OnEnter(this);
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

void BurgerComponent::HandleHitByObject(GameObject* pFallingObject)
{
	const auto pRigidBodyCompObject{ pFallingObject->GetComponent<RigidBodyComponent>() };
	const auto pBurgerRigidBodyCompShape{ pRigidBodyCompObject->GetShape() };

	if (!m_pShape->CollidesWith(pBurgerRigidBodyCompShape))
	{
		return;
	}
	const auto pObjectBurgerComponent{ pFallingObject->GetComponent<BurgerComponent>() };
	if (pObjectBurgerComponent && m_State != BurgerComponent::State::falling)
	{
		//Never set the burger class directly with a state class. I lost quite some time figuring out why
		//my ingredients were not landing on the platforms. It still remains a mystery
		SetState(BurgerComponent::State::falling);
	}
}

void BurgerComponent::HandleInput()
{
	if (const auto newState = m_pBurgerState->HandleInput())
	{
		m_pBurgerState->OnExit(this);
		m_pBurgerState = std::unique_ptr<BurgerState>(newState);
		m_pBurgerState->OnEnter(this);
	}
}

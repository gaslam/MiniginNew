#include "BurgerComponent.h"
#include "GameObject.h"
#include "Shape.h"
#include "../Components/CharacterComponent.h"
#include "../Components/RenderComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../States/BurgerState.h"
#include "../States/BurgerStandingState.h"

using namespace dae;

BurgerComponent::BurgerComponent(GameObject* pObject, const std::string& file) : Component{ pObject }, m_pBurgerState{std::make_unique<BurgerStandingState>()}
{
	auto pOwner{ GetOwner() };
	m_pRenderComp = pOwner->GetComponent<RenderComponent>();
	if (!m_pRenderComp)
	{
		m_pRenderComp = pOwner->AddComponent<RenderComponent>(file);
	}
	m_pRenderComp->SetScale(2.5f);
	Transform* pTransform{ pOwner->GetComponent<Transform>() };
	if (!pTransform)
	{
		pTransform = pOwner->AddComponent<Transform>();
	}
	const int width{ static_cast<int>(m_pRenderComp->GetFrameWidthScaled()) };
	const int height{ static_cast<int>(m_pRenderComp->GetFrameHeightScaled()) };
	const glm::ivec2 pos{pTransform->GetLocalPosition()};
	m_pShape = new RectangleShape{ pos, width,height };
	const auto pRigidBody{ pOwner->AddComponent<RigidBodyComponent>(pTransform) };
	pRigidBody->SetShape(m_pShape);
	m_pBurgerState->OnEnter(this);
}

void BurgerComponent::Update(float deltaTime)
{
	HandleInput();
	m_pBurgerState->Update(this,deltaTime);
}

void dae::BurgerComponent::SetDegreesTurned(double degrees)
{
	m_DegreesTurned = degrees;
	m_pRenderComp->SetAngle(m_DegreesTurned);
}

void dae::BurgerComponent::HandleInput()
{
	const auto newState = m_pBurgerState->HandleInput();
	if(newState)
	{
		m_pBurgerState->OnExit(this);
		m_pBurgerState = std::unique_ptr<BurgerState>(newState);
		m_pBurgerState->OnEnter(this);
	}
}

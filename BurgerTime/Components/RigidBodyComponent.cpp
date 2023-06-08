#include "RigidBodyComponent.h"

#include "GameObject.h"
#include "Shape.h"
#include "Logger.h"
#include "Transform.h"
void dae::RigidBodyComponent::Render() const
{
	m_Shape->Render();
}

void dae::RigidBodyComponent::Update(float elapsedSec)
{
	MG_ASSERT(m_pTransform != nullptr, "Cannot get transform!!");
	if(m_pTransform)
	{
		const auto pos = m_pTransform->GetWorldPosition();
		auto posVec2 = glm::ivec2{ pos.x ,pos.y };
		m_Shape->SetPosition(posVec2);
	}

	if(!m_IsStatic)
	{
		m_TotalTimePassed += elapsedSec;
		const float elapsedSecSquare{ glm::pow(m_TotalTimePassed,2.f) };
		auto pTransform = GetOwner()->GetComponent<Transform>();
		auto localPos = pTransform->GetWorldPosition();
		localPos.y -= (m_Gravity * elapsedSecSquare)/2.f;
		pTransform->SetLocalPosition(localPos);
	}
}

void dae::RigidBodyComponent::SetShape(Shape* shape)
{
	m_Shape = std::unique_ptr<Shape>(shape);
}

bool dae::RigidBodyComponent::IsPointInRect(glm::ivec2& point) const
{
	return m_Shape->IsPointIn(point);
}

bool dae::RigidBodyComponent::IsOverlapping(RigidBodyComponent* pComponent) const
{
	auto pShape = pComponent->GetShape();
	MG_ASSERT(pShape == nullptr,"Cannot get shape!!");
	return m_Shape->CollidesWith(pShape);
}

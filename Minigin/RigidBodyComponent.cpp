#include "RigidBodyComponent.h"
#include "Shape.h"
#include "Logger.h"
#include "Transform.h"
void dae::RigidBodyComponent::Render() const
{
	m_Shape->Render();
}

void dae::RigidBodyComponent::Update(float)
{
	MG_ASSERT(m_pTransform != nullptr);
	if(m_pTransform)
	{
		const auto pos = m_pTransform->GetWorldPosition();
		auto posVec2 = glm::ivec2{ pos.x ,pos.y };
		m_Shape->SetPosition(posVec2);
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
	MG_ASSERT(pShape == nullptr);
	return m_Shape->CollidesWith(pShape);
}

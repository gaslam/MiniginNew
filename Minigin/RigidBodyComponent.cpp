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

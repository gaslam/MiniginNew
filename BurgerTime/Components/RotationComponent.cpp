#include <windows.h>
#include "RotationComponent.h"
#include "GameObject.h"
#include "Transform.h"

#define PI 3.14159265358979323846f

dae::RotationComponent::RotationComponent(GameObject* owner, float radius, float speedInRad) : Component(owner), m_Radius{ radius }, m_RotSpeed{ speedInRad }
{
	if (!owner)
	{
		return;
	}
	m_TransformComp = owner->AddComponent<Transform>();
	MG_ASSERT(m_TransformComp != nullptr,"Cannot get transform!!");
	if (m_TransformComp)
	{
		m_Center = m_TransformComp->GetLocalPosition();
	}
}

void dae::RotationComponent::Render() const
{
}

void dae::RotationComponent::Update(float deltaTime)
{
	if (!m_TransformComp)
	{
		//todo: Place warning later. Ask teacher whether I can already implement
		return;
	}
	m_CurrentRadians += m_RotSpeed * deltaTime;

	const float newYPosRot{ sinf(m_CurrentRadians) * m_Radius };
	const float newXPosRot{ cosf(m_CurrentRadians) * m_Radius };

	glm::vec3 newPos{ newXPosRot + m_Center.x, newYPosRot + m_Center.y, 0 };

	m_TransformComp->SetLocalPosition(newPos);
}

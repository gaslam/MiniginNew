#include "Transform.h"
#include <algorithm>
#include "GameObject.h"

const glm::vec3& dae::Transform::GetWorldPosition()
{
	if (m_PositionIsDirty)
	{
		UpdateWorldPosition();
	}
	return m_WorldPosition;
}

void dae::Transform::UpdateWorldPosition()
{
	if (m_PositionIsDirty)
	{
		auto owner = GetOwner();
		if (owner == nullptr)
		{
			return;
		}
		auto parent{ owner->GetParent() };
		if (!parent)
		{
			m_WorldPosition = m_LocalPosition;
			return;
		}
		auto transform = parent->GetComponent<Transform>();
		if (transform)
		{
			m_WorldPosition = transform->GetWorldPosition() + m_LocalPosition;
		}
		m_PositionIsDirty = false;
	}
}

void dae::Transform::SetLocalPosition(const glm::vec3& pos)
{
	m_LocalPosition = pos;
	SetPositionDirty();
}

void dae::Transform::SetPositionDirty()
{
	m_PositionIsDirty = true;
}

//void dae::Transform::SetPosition(const float x, const float y, const float z)
//{
//	m_position.x = x;
//	m_position.y = y;
//	m_position.z = z;
//}

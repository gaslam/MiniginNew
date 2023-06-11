#include "Transform.h"
#include "GameObject.h"

using namespace dae;
const glm::vec2& Transform::GetWorldPosition()
{
	if (m_PositionIsDirty)
	{
		UpdateWorldPosition();
	}
	return m_WorldPosition;
}

void Transform::UpdateWorldPosition()
{
	if (m_PositionIsDirty)
	{
		auto owner = GetOwner();
		if (owner == nullptr)
		{
			return;
		}
		auto parent{ owner->GetParent() };
		if (parent == nullptr)
		{
			m_WorldPosition = m_LocalPosition;
		}
		else
		{
			auto transform = parent->GetComponent<Transform>();
			if (transform)
			{
				m_WorldPosition = transform->GetWorldPosition() + m_LocalPosition;
			}
		}
		m_PositionIsDirty = false;
	}
}

void Transform::SetLocalPosition(const glm::vec2& pos)
{
	m_LocalPosition = pos;
	SetPositionDirty();
}

void Transform::SetPositionDirty()
{
	m_PositionIsDirty = true;
}

Transform* Transform::Clone(GameObject* pObject)
{
	if(pObject->IsComponentAdded<Transform>())
	{
		pObject->RemoveComponent<Transform>();
	}
	const auto transform{ pObject->AddComponent<Transform>() };
	transform->SetWorldPosition(m_WorldPosition);
	return transform;
}

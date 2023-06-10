#include "GameObject.h"
#include "Renderer.h"
#include <algorithm>
#include <ranges>

#include "Component.h"


void dae::GameObject::Update(float deltaTime)
{
	for (auto& component : m_Components | std::views::values)
	{
		component->Update(deltaTime);
	}

	for (const auto& child : m_Children)
	{
		child->Update(deltaTime);
	}
}

void dae::GameObject::Start()
{
	for (const auto& component : m_Components | std::views::values)
	{
		component->Start();
	}
}

void dae::GameObject::End()
{
	for (const auto& component : m_Components | std::views::values)
	{
		component->End();
	}
}

void dae::GameObject::Render() const
{

	for (const auto& component : m_Components | std::views::values)
	{
		component->Render();
	}
}

void dae::GameObject::RenderImGUI() const
{
	for (const auto& component : m_Components | std::views::values)
	{
		component->RenderImGUI();
	}
}

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition = false)
{
	const auto transform = GetComponent<Transform>();
	if (transform == nullptr)
	{
		return;
	}
	if (m_Parent == nullptr)
	{
		transform->SetLocalPosition(transform->GetWorldPosition());
	}
	else
	{
		const auto transformParent = parent == nullptr ? nullptr : parent->GetComponent<Transform>();
		if (keepWorldPosition && transformParent != nullptr)
		{
			transform->SetLocalPosition(transform->GetLocalPosition() - transformParent->GetWorldPosition());
		}
		if (transform)
		{
			transform->SetPositionDirty();
			for (auto& child : m_Children)
			{
				if (const auto childTransform = child->GetComponent<Transform>())
				{
					childTransform->SetPositionDirty();
				}
			}
		}
	}

	if (m_Parent)
	{
		m_Parent->RemoveChild(this);
	}
	m_Parent = parent;
	if (m_Parent)
	{
		m_Parent->AddChild(this);
	}
}

void dae::GameObject::RemoveChild(GameObject* child)
{
	const auto foundObject = std::find(m_Children.begin(), m_Children.end(), child);
	if (foundObject != m_Children.end())
	{
		m_Children.erase(foundObject);
	}
}

void dae::GameObject::AddChild(GameObject* child)
{
	m_Children.emplace_back(child);
}

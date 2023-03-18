#include <string>
#include "GameObject.h"
#include "Renderer.h"
#include "RenderComponent.h"
#include "Texture2D.h"
#include <algorithm>
#include "Component.h"


dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(float deltaTime)
{
	for (auto& component : m_Components)
	{
		component.second->Update(deltaTime);
	}

	for (auto& child : m_Children)
	{
		child->Update(deltaTime);
	}
}

void dae::GameObject::Render() const
{

	for (auto& component : m_Components)
	{
		component.second->Render();
	}
}

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition = false)
{
	auto transform = GetComponent<Transform>();
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
		auto transformParent = parent == nullptr ? nullptr : parent->GetComponent<Transform>();
		if (keepWorldPosition && transformParent != nullptr)
		{
			transform->SetLocalPosition(transform->GetLocalPosition() - transformParent->GetWorldPosition());
		}
		if (transform)
		{
			transform->SetPositionDirty();
			for (auto& child : m_Children)
			{
				if (auto childTransform = child->GetComponent<Transform>())
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
	auto foundObject = std::find(m_Children.begin(), m_Children.end(), child);
	if (foundObject != m_Children.end())
	{
		m_Children.erase(foundObject);
	}
}

void dae::GameObject::AddChild(GameObject* child)
{
	m_Children.emplace_back(child);
}

#pragma once
#include <memory>
#include "Transform.h"
#include <vector>
#include "string"
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <vector>

class Component;
namespace dae
{

	// todo: this should become final.
	class GameObject final
	{
	public:
		virtual void Update(float deltaTime);
		virtual void Render() const;

		void SetParent(GameObject* parent, bool keepWorldPosition);
		void RemoveChild(GameObject* child);
		void AddChild(GameObject* child);

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		template <typename T, typename... Args> T* AddComponent(Args&&... args) {
			if (IsComponentAdded<T>()) {
				return GetComponent<T>();
			}

			const std::type_index typeIndex = std::type_index(typeid(T));
			auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
			auto pointer = dynamic_cast<T*>(component.get());

			m_Components.emplace(typeIndex, std::move(component));

			return pointer;
		};


		template<typename T>
		T* GetComponent() const
		{
			const std::type_index typeIndex = std::type_index(typeid(T));
			if (!IsComponentAdded<T>())
			{
				return nullptr;
			}
			auto component = dynamic_cast<T*>(m_Components.at(typeIndex).get());
			return component;
		}
		template<typename T>
		void RemoveComponent()
		{
			const std::type_index typeIndex = std::type_index(typeid(T));
			auto component = m_Components.at(typeIndex);

			if (component)
			{
				m_Components.erase(typeIndex);
			}
		}

		template<typename T>
		bool IsComponentAdded() const
		{
			const std::type_index typeIndex = std::type_index(typeid(T));

			return m_Components.contains(typeIndex);
		}

		GameObject* GetParent() const { return m_Parent; };

	private:
		std::unordered_map<std::type_index,std::unique_ptr<Component>> m_Components{};

		GameObject* m_Parent{ nullptr };
		std::vector<GameObject*> m_Children{};
	};
}

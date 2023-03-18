#pragma once
#include <glm/glm.hpp>
namespace dae {
	class GameObject;
	class Component {
	public:
		virtual ~Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;
		virtual void Render() const = 0;
		virtual void Update(float deltaTime) = 0;
	protected:
		GameObject* GetOwner() const { return m_pOwner; }
		explicit Component(GameObject* owner) : m_pOwner{ owner } {};
	private:
		GameObject* m_pOwner{ nullptr };
	};
}

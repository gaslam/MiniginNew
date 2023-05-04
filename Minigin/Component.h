#pragma once
#include "Logger.h"
<<<<<<< HEAD
#include <windows.h>
=======
>>>>>>> 9e333116ee788fa22f3985d3b449ca2ed1330109
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
		explicit Component(GameObject* owner) : m_pOwner{ owner } {
			MG_ASSERT(owner != nullptr);
		};
	private:
		GameObject* m_pOwner{ nullptr };
	};
}

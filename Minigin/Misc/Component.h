#pragma once
#include <windows.h>
#include "Logger.h"
namespace dae {
	class GameObject;
	class Component {
	public:
		virtual ~Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) noexcept = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) noexcept = delete;
		virtual void Render() const {};
		virtual void Update(float) {};
		virtual void RenderImGUI() {};
		virtual void Start(){}
		virtual void End(){}

	protected:
		GameObject* GetOwner() const { return m_pOwner; }
		explicit Component(GameObject* owner) : m_pOwner{ owner } {
			MG_ASSERT(owner != nullptr,"Cannot get owner!!");
		};
	private:
		GameObject* m_pOwner{ nullptr };
	};
}

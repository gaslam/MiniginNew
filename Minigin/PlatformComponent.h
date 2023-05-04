#pragma once
#include "Component.h"

namespace dae
{
	class RectangleShape;
	class RigidBodyComponent;
	class PlatformComponent :
		public Component
	{
	public:
		PlatformComponent(GameObject* pOwner, RectangleShape* pShape);
		void Render() const override {};
		void Update(float deltaTime);
	private:
		RigidBodyComponent* m_pRigidBodyComp{};
	};
}


#pragma once
#include <Misc/Component.h>

namespace dae
{
	class RectangleShape;
	class RigidBodyComponent;
	class PlatformComponent :
		public Component
	{
	public:
		PlatformComponent(GameObject* pOwner, RectangleShape* pShape);
		void Update(float) override;
		void HandleHit(GameObject* pFallingObject);
	private:
		RigidBodyComponent* m_pRigidBodyComp{};
	};

}


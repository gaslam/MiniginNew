#pragma once
#include <Misc/Component.h>

namespace dae
{
	class RectangleShape;
	class RigidBodyComponent;
	class LadderComponent :
		public Component
	{
	public:
		LadderComponent(GameObject* pOwner, RectangleShape* pShape);
		void Render() const override {};
		void Update(float deltaTime);
	private:
		RigidBodyComponent* m_pRigidBodyComp{};

	};
}


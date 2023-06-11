#pragma once
#include <Misc/Observer.h>

namespace dae
{
	class RigidBodyComponent;

	class PlatformObserver :
    public Observer
{
	public:
		PlatformObserver(RigidBodyComponent* pComponent) : m_pRigidBody{pComponent} {}
		virtual void OnNotify(GameObject* object, Event& event) override;
	private:
		void HandleHit(GameObject* pGameObject);
		void HandleFall(GameObject* pGameObject);
		RigidBodyComponent* m_pRigidBody{};
};
}



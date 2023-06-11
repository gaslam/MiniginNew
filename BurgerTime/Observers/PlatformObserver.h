#pragma once
#include <Misc/Observer.h>

namespace dae
{

	class PlatformComponent;
	class PlatformObserver :
    public Observer
{
	public:
		PlatformObserver(PlatformComponent* pComponent) : m_pComponent{pComponent} {}
		virtual void OnNotify(GameObject* object, Event& event) override;
	private:
		void HandleFall(GameObject* pGameObject);
		PlatformComponent* m_pComponent{};
};
}



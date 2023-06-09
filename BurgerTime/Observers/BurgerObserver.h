#pragma once
#include <Observer.h>
namespace dae
{
	class BurgerComponent;

	class BurgerObserver :
        public Observer
    {
	public:
        BurgerObserver(BurgerComponent* component) : m_pBurgerComponent{component}{}
        void HandleFall(GameObject* pGameObject);
		void OnNotify(GameObject* object, Event& event) override;
        BurgerComponent* GetComponent() { return m_pBurgerComponent; }
    private:
        BurgerComponent* m_pBurgerComponent{};
    };
}


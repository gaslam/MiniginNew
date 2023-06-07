#pragma once
#include "BurgerState.h"

namespace dae
{
	class RectangleShape;
    class BurgerComponent;
    class BurgerStandingState :
        public BurgerState
    {
    public:
        void Update(BurgerComponent*, float) override {}
        BurgerState* HandleInput() override;
        void OnEnter(BurgerComponent* pComponent) override;
    private:
        dae::RectangleShape* m_pShape{};
    };
}

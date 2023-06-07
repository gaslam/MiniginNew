#pragma once
#include "BurgerState.h"
namespace dae
{
    class BurgerFallingState :
        public BurgerState
    {
    public:
        BurgerState* HandleInput() override;
        void OnEnter(BurgerComponent* pComponent) override;
        void Update(BurgerComponent* pComponent, float deltaTime) override;
    };
}


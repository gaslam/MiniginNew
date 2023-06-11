#pragma once
#include <glm/vec2.hpp>
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
    private:
        bool m_IsStateFalling{};
        float m_DistanceToTravel{};
        glm::vec2 m_StartPos{};
        glm::vec2 m_CurrentPos{};
    };
}


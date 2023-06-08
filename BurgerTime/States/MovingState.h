#pragma once
#include "PlayerState.h"
namespace dae
{
    class CharacterComponent;
    class MovingState :
        public CharacterState
    {
    public:
        MovingState(const glm::vec2& dir) : m_Dir(dir) {}
        void Update(CharacterComponent* pComponent, float deltaTime) override;
        CharacterState* HandleInput() override;
    private:
        glm::vec2 m_Dir;
    };
}


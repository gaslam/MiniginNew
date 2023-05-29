#pragma once
#include "PlayerState.h"
namespace dae
{
	class CharacterComponent;
class IdleState :
    public CharacterState
{
public:
    void Update(CharacterComponent*, float) override{}
    CharacterState* HandleInput() override;
    void OnEnter(CharacterComponent* pComponent) override;
};
}



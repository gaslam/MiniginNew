#pragma once
#include <Component.h>

#include "GameObject.h"

namespace dae
{
	class BurgerComponent :
    public Component
{
	public:
		BurgerComponent(GameObject* pObject, const std::string& file);
		void Update(float) override;
	private:
		float m_DegreesTurned{};
		float m_TotalDegrees{};
		enum class State
		{
			standingStill,
			falling
		};
};
}



#pragma once
#include "BurgerState.h"
namespace dae
{
	class BurgerTurningState :
		public BurgerState
	{
	public:
		BurgerState* HandleInput() override;
		void Update(BurgerComponent* pComponent, float deltaTime) override;
		void OnEnter(BurgerComponent* pComponent) override;
		void OnExit(BurgerComponent* pComponent) override;
	private:
		bool m_PlayerLeft{false};
		//has to be double to avoid casting the double variable for SDL
		double m_DegreesTurned{};
		double m_AngSpeed{ 10 };
		double m_TotalDegrees{20};
	};
}



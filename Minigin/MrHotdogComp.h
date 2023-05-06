#pragma once
#include "EnemyComponent.h"
namespace dae
{
	class MrHotdogComp final:
		public EnemyComponent
	{
	public:
		explicit MrHotdogComp(GameObject* pObject);

		void Render() const override{};
		void Update(float) override{};

		EnemyComponent* Clone(GameObject* object) override;
	};
}


#pragma once
#include "EnemyComponent.h"
namespace dae
{
	class MrEggComponent :
		public EnemyComponent
	{
	public:
		explicit MrEggComponent(GameObject* pObject);

		void Render() const override {};
		void Update(float) override {};

		EnemyComponent* Clone(GameObject* object) override;
	};
}


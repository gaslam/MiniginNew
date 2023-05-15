#pragma once
#include "EnemyComponent.h"
namespace dae
{
	class EnemyComponent;

	class MrPickleComponent :
		public EnemyComponent
	{
	public:
		explicit MrPickleComponent(GameObject* pObject);

		void Render() const override {};
		void Update(float) override {};

		EnemyComponent* Clone(GameObject* object) override;
	};
}


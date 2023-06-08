#pragma once
#include <Component.h>

namespace dae
{
	class EnemyComponent :
		public Component
	{
	public:
		explicit EnemyComponent(GameObject* object);
		~EnemyComponent() override = default;

		EnemyComponent(const EnemyComponent& other) = delete;
		EnemyComponent(EnemyComponent&& other) noexcept = delete;
		EnemyComponent& operator=(const EnemyComponent& other) = delete;
		EnemyComponent& operator=(EnemyComponent&& other) noexcept = delete;
		void Render() const override{}
		void Update(float) override{}
		virtual EnemyComponent* Clone(GameObject* object);
	};
}


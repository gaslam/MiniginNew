#pragma once
#include <Misc/Component.h>
#include <glm/vec2.hpp>

#include <Misc/Subject.h>
#include "GridComponent.h"

namespace dae
{
	class EnemyComponent :
		public Component, public Subject
	{
	public:
		explicit EnemyComponent(GameObject* object);
		~EnemyComponent() override = default;

		EnemyComponent(const EnemyComponent& other) = delete;
		EnemyComponent(EnemyComponent&& other) noexcept = delete;
		EnemyComponent& operator=(const EnemyComponent& other) = delete;
		EnemyComponent& operator=(EnemyComponent&& other) noexcept = delete;
		void Render() const override{}
		void Update(float) override;
		virtual EnemyComponent* Clone(GameObject* object);
		void CalculatePath(GameObject* target);
		void SetGrid(GridComponent* gridComponent);
	private:
		glm::vec2 m_MoveDir{};
		GridComponent* m_pGridComponent{};
		std::vector<Node*> m_Path{};
	};
}


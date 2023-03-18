#pragma once
#include <glm/glm.hpp>
#include "Component.h"

namespace dae
{
	class Transform : public Component
	{
	public:
		Transform(GameObject* owner) : Component(owner) {};
		const glm::vec3 GetLocalPosition() const { return m_LocalPosition; }
		const glm::vec3& GetWorldPosition();
		void UpdateWorldPosition();
		//void SetPosition(float x, float y, float z);
		void SetLocalPosition(const glm::vec3& pos);
		void SetWorldPosition(const glm::vec3& pos) { m_WorldPosition = pos; }
		void SetPositionDirty();

		virtual void Render() const override {};
		virtual void Update(float) override {};
	private:
		glm::vec3 m_LocalPosition{};
		glm::vec3 m_WorldPosition{};
		bool m_PositionIsDirty{ false };
	};
}

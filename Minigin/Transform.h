#pragma once
#include <glm/glm.hpp>
#include "Component.h"

namespace dae
{
	class Transform : public Component
	{
	public:
		Transform(GameObject* owner) : Component(owner) {};
		const glm::vec2 GetLocalPosition() const { return m_LocalPosition; }
		const glm::vec2& GetWorldPosition();
		void UpdateWorldPosition();
		//void SetPosition(float x, float y, float z);
		void SetLocalPosition(const glm::vec2& pos);
		void SetWorldPosition(const glm::vec2& pos) { m_WorldPosition = pos; }
		void SetPositionDirty();

		virtual void Render() const override {};
		virtual void Update(float) override {};
		float GetSpeedForMovement() const { return m_SpeedForMovement; }
		void SetSpeedForMovement(float speed) { m_SpeedForMovement = speed; }
	private:
		glm::vec2 m_LocalPosition{};
		glm::vec2 m_WorldPosition{};
		//Did not know where to put this. Is it also not bad to have a speed component just for this?
		float m_SpeedForMovement{20.f};
		bool m_PositionIsDirty{ false };
	};
}

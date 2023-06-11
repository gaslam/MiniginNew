#pragma once
#include <glm/glm.hpp>

#include <Misc/Component.h>
namespace dae {
	class GameObject;
	class Transform;
	class RotationComponent :
		public Component
	{
	public:
		RotationComponent(GameObject* owner, float radius,float rotSpeed);
		virtual void Render() const override;
		virtual void Update(float deltaTime) override;
		void SetRadius(float radius) { m_Radius = radius; }
		float GetRadius() const { return m_Radius; }
		void SetRotationSpeed(float speed) { m_RotSpeed = speed; }
		float GetRotSpeed() const { return m_RotSpeed; }
	private:
		float m_Radius{};
		float m_RotSpeed{};
		float m_CurrentRadians{};
		glm::ivec2 m_Center{};

		Transform* m_TransformComp{ nullptr };
	};

}

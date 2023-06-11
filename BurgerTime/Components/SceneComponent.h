#pragma once
#include <glm/vec2.hpp>
#include <Misc/Component.h>
namespace dae
{
	class AudioComponent;

	class SceneComponent :
		public Component
	{
	public:
		SceneComponent(GameObject* gameObject, glm::vec2 playerPos = {},float worldScale =1.f);
		void Start() override;
		void End() override;
		void SetPlayerSpawnPos(const glm::vec2 playerPos, float worldScale) { m_PlayerPos = playerPos * worldScale; }
	private:
		AudioComponent* m_pBackgroundSound{};
		glm::vec2 m_PlayerPos{};
	};
}



#pragma once
#include <vector>
#include <string>
#include <memory>
#include "../Misc/Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Update(float deltaTime);
		void FixedUpdate(float deltaTime);
		void Render();
		void SetScene(int scene);
		Scene* GetScene() const { return m_scenes[m_Scene].get(); }
		void GoToNextScene();
		void GoToPreviousScene();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;
		int m_Scene{};
	};
}

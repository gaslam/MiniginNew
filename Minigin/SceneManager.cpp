#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update(float deltaTime)
{
	m_scenes[m_ActiveScene]->Update(deltaTime);
}

void dae::SceneManager::FixedUpdate(float)
{
}

void dae::SceneManager::Render()
{
	m_scenes[m_ActiveScene]->Render();
}

void dae::SceneManager::SetActiveScene(int sceneId)
{
	if (m_ActiveScene >= static_cast<int>(m_scenes.size()))
	{
		return;
	}

	m_ActiveScene = sceneId;
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}

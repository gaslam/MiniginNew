#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update(float deltaTime)
{
	m_scenes[m_Scene]->Update(deltaTime);
}

void dae::SceneManager::FixedUpdate(float)
{
}

void dae::SceneManager::Render()
{
	m_scenes[m_Scene]->Render();
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}

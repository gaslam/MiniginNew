#include "SceneManager.h"
#include "Scene.h"
#include <imgui.h>

using namespace dae;

void SceneManager::Update(float deltaTime)
{
	m_scenes[m_Scene]->Update(deltaTime);
}

void SceneManager::FixedUpdate(float)
{
}

void SceneManager::Render()
{
	m_scenes[m_Scene]->Render();
	ImGui::SetNextWindowSize(ImVec2(240, 250));
	ImGui::Begin("Burger time debug: ", nullptr, ImGuiWindowFlags_None);
	m_scenes[m_Scene]->RenderImGUI();
	ImGui::End();
}

void SceneManager::SetScene(int scene)
{
	m_scenes[m_Scene]->End();
	m_Scene = scene;
	m_scenes[m_Scene]->Start();
}

void SceneManager::GoToPreviousScene()
{
	if (m_Scene - 1 < 0) return;
	m_scenes[m_Scene]->End();
	--m_Scene;
	m_scenes[m_Scene]->Start();
}

void SceneManager::GoToNextScene()
{
	if (m_Scene + 1 >= static_cast<int>(m_scenes.size())) return;
	m_scenes[m_Scene]->End();
	++m_Scene;
	m_scenes[m_Scene]->Start();
}

Scene& SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}

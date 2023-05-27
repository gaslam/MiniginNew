#include "SceneManager.h"
#include "Scene.h"
#include <imgui.h>

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
	ImGui::SetNextWindowSize(ImVec2(240, 250));
	ImGui::Begin("Burger time debug: ", nullptr, ImGuiWindowFlags_None);
	m_scenes[m_Scene]->RenderImGUI();
	ImGui::End();
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}

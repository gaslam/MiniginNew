#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include <memory>
#include "RenderComponent.h"
#include "FPSComponent.h"
#include "RotationComponent.h"
#include "Input.h"
#include "ImGuiSpeedSliderComp.h"
#include <string>
#include "Renderer.h"

void prepareControlsWithGameObjects(dae::Scene&) {
	glm::vec3 up = { 0.f,-1.f,0.f };
	glm::vec3 down = { 0.f,1.f,0.f };
	glm::vec3 right = { 1.f,0.f,0.f };
	glm::vec3 left = { -1.f,0.f,0.f };
	//auto& inputInstance = dae::Input::GetInstance();
}

void AddScene(int stageId)
{
	std::string stageIdStr{ std::to_string(stageId) };
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Stage " + stageIdStr);

	auto go = std::make_shared<dae::GameObject>();
	auto render = go->AddComponent<dae::RenderComponent>("Backgrounds/Stage_" + stageIdStr + ".png");
	render->scale(2.4f);
	auto transform = go->AddComponent<dae::Transform>();
	glm::ivec2 windowSize = dae::Renderer::GetInstance().GetWindowWidthAndHeight();
	const float backgroundWidth = render->GetWidth();
	const float backgroundHeight = render->GetWidth();
	transform->SetLocalPosition({ windowSize.x / 2.f - backgroundWidth / 2.f,windowSize.y - backgroundHeight, 0.f });
	scene.Add(go);
}

void load()
{
	AddScene(1);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}
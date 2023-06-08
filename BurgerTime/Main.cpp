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
#include "GameObject.h"
#include "Scene.h"
#include <memory>
#include <string>

#include "Components/BurgerComponent.h"
#include "CharacterManager.h"
#include "Locator.h"
#include "Renderer.h"
#include "Components/RenderComponent.h"
#include "Components/MrEggComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/SpawnerComponent.h"
#include "Utils.h"
#include "Components/GridComponent.h"

void AddScene(int stageId)
{
	std::string stageIdStr{ std::to_string(stageId) };
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Stage " + stageIdStr);

	auto go = std::make_shared<dae::GameObject>();
	auto render =go->AddComponent<dae::RenderComponent>("Backgrounds/Stage_" + stageIdStr + ".png");
	const float worldScale{3.f};
	render->SetScale(worldScale);
	auto transform = go->AddComponent<dae::Transform>();
	glm::ivec2 windowSize = dae::Renderer::GetInstance().GetWindowWidthAndHeight();
	const float backgroundWidth = render->GetWidthScaled();
	const float backgroundHeight = render->GetHeightScaled();
	scene.Add(go);
	glm::vec2 worldPos = { windowSize.x / 2.f - backgroundWidth / 2.f,windowSize.y - backgroundHeight };
	transform->SetWorldPosition(worldPos);
	std::string file{ "../Data/LevelData/Stage" + stageIdStr + "Data.btf" };
	Utils::ReadLevelData(file, &scene,worldPos,worldScale);

	scene.Add(dae::CharacterManager::GetInstance().InitPlayer());

	auto audio = Locator::GetAudio();
	const int id = audio->Load("Sound/background.wav");
	audio->Play(id,-1);

	go = std::make_shared<dae::GameObject>();
	glm::vec2 spawnPos = glm::vec2{ 50, 50 };
	go->AddComponent<dae::SpawnerComponent<dae::MrEggComponent>>(spawnPos);
	scene.Add(go);
}

void load()
{
	MG_ASSERT_INFO("Starting load Process!!");
	MG_ASSERT_INFO("Adding scene 1!!");
	AddScene(1);
}

int main(int, char*[]) {
	MG_ASSERT_INFO("Starting program!!");
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}
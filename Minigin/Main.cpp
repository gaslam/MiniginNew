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
#include <steam_api.h>
#include <iostream>
#include "HealthComponent.h"
#include "RenderComponent.h"
#include "TextComponent.h"

void prepareControlsWithGameObjects(dae::Scene& scene) {
	auto character = std::make_shared<dae::GameObject>();
	character->AddComponent<dae::HealthComponent>(3);
	auto transform = character->AddComponent<dae::Transform>();
	transform->SetLocalPosition({ 25, 425,0 });
	scene.Add(character);
}

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>("background.tga",true);
	go->AddComponent<dae::Transform>();
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>("logo.tga",true);
	auto transform = go->AddComponent<dae::Transform>();
	transform->SetLocalPosition({ 216, 180,0 });
	scene.Add(go);
	prepareControlsWithGameObjects(scene);
}

int main(int, char*[]) {
	if (!SteamAPI_Init())
	{
		std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)." << std::endl;
	}
	else
		std::cout << "Successfully initialized steam." << std::endl;

		SteamAPI_Shutdown();

	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}
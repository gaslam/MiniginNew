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
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "InputManager.h"
#include "Input.h"
#include "Observer.h"

void prepareControlsWithGameObjects(dae::Scene& scene) {
	glm::vec3 pos{ 25, 425,0 };
	const int totalCharacters{ 2 };
	const int totalSupportedKeys{ totalCharacters * 2 };
	const float yPosToSubtract{ 50.f };
	int scanCodeIdx{};
	const SDL_Scancode codes[totalSupportedKeys]{ SDL_SCANCODE_S,SDL_SCANCODE_W,SDL_SCANCODE_DOWN,SDL_SCANCODE_UP };
	int index{};
	do {
		auto character = std::make_shared<dae::GameObject>();
		auto characterHealth = new dae::GameObject{};
		auto transform = characterHealth->AddComponent<dae::Transform>();

		transform->SetWorldPosition(pos);
		characterHealth->AddComponent<dae::HealthComponent>(3);
		character->AddChild(characterHealth);

		auto characterScore = new dae::GameObject{};
		characterScore->AddComponent<dae::ScoreComponent>();
		transform = characterScore->AddComponent<dae::Transform>();
		pos.y -= yPosToSubtract;
		transform->SetWorldPosition(pos);
		character->AddChild(characterScore);

		dae::DamageCommand* attackCommand{ new dae::DamageCommand{characterHealth} };
		dae::ScoreCommand* scoreCommand{ new dae::ScoreCommand{characterScore} };
		auto controllerButton = dae::XboxController::ControllerButton::A;
		auto keyBoardKey =  codes[scanCodeIdx];
		++scanCodeIdx;
		auto keyState = dae::KeyState::pressed;
		dae::Input::GetInstance().AddController(index);
		dae::Input::GetInstance().BindButtonsToCommand(index, controllerButton, keyBoardKey, keyState, attackCommand);
		controllerButton = dae::XboxController::ControllerButton::B;
		keyBoardKey = codes[scanCodeIdx];
		++scanCodeIdx;
		keyState = dae::KeyState::pressed;
		dae::Input::GetInstance().BindButtonsToCommand(index, controllerButton, keyBoardKey, keyState, scoreCommand);

		scene.Add(character);
		pos.y -= yPosToSubtract;
		++index;
	} while (index < totalCharacters);
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
	transform->SetWorldPosition({ 216, 180,0 });
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
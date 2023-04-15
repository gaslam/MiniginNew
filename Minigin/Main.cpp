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
#include "RotationComponent.h"
#include "ImGuiTutorial.h"

void prepareControlsWithGameObjects(dae::Scene& scene) {
	glm::vec3 pos{ 25, 425,0 };
	const int totalCharacters{ 2 };
	const int totalSupportedKeys{ totalCharacters * 2 };
	const float yPosToSubtract{ 50.f };
	int scanCodeIdx{};
	const SDL_Scancode codes[totalSupportedKeys]{ SDL_SCANCODE_S,SDL_SCANCODE_W,SDL_SCANCODE_DOWN,SDL_SCANCODE_UP };

	//Green,Yellow
	const SDL_Color playerColors[totalCharacters]{ SDL_Color{0,255,0},SDL_Color{255,255,0} };
	auto keyState = dae::KeyState::pressed;
	int playerLives{ 3 };
	int index{};
	do {
		auto character = std::make_shared<dae::GameObject>();
		auto characterHealth = std::make_shared<dae::GameObject>();
		auto transform = characterHealth->AddComponent<dae::Transform>();
		scene.Add(characterHealth);

		transform->SetWorldPosition(pos);
		characterHealth->AddComponent<dae::HealthComponent>(playerLives, playerColors[index]);
		character->AddChild(characterHealth.get());

		auto characterScore = std::make_shared<dae::GameObject>();
		characterScore->AddComponent<dae::ScoreComponent>(playerColors[index]);
		transform = characterScore->AddComponent<dae::Transform>();
		pos.y -= yPosToSubtract;
		transform->SetWorldPosition(pos);
		character->AddChild(characterScore.get());
		scene.Add(characterScore);

		dae::DamageCommand* attackCommand{ new dae::DamageCommand{characterHealth.get()}};
		dae::ScoreCommand* scoreCommand{ new dae::ScoreCommand{characterScore.get()}};
		auto controllerButton = dae::XboxController::ControllerButton::B;
		auto keyBoardKey =  codes[scanCodeIdx];
		++scanCodeIdx;
		dae::Input::GetInstance().AddController(index);
		dae::Input::GetInstance().BindButtonsToCommand(index, controllerButton, keyBoardKey, keyState, attackCommand);
		controllerButton = dae::XboxController::ControllerButton::A;
		keyBoardKey = codes[scanCodeIdx];
		++scanCodeIdx;
		dae::Input::GetInstance().BindButtonsToCommand(index, controllerButton, keyBoardKey, keyState, scoreCommand);

		scene.AddPlayer(character);
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

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>("logo.tga", true);
	go->AddComponent<dae::ImGuiTutorial>();
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

	dae::Minigin engine("../Data/");
	engine.Run(load);
	SteamAPI_Shutdown();
    return 0;
}
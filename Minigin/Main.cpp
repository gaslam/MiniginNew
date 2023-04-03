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
#include <steam_api.h>
#include <iostream>

void prepareControlsWithGameObjects(dae::Scene& scene) {
	glm::vec3 up = { 0.f,-1.f,0.f };
	glm::vec3 down = { 0.f,1.f,0.f };
	glm::vec3 right = { 1.f,0.f,0.f };
	glm::vec3 left = { -1.f,0.f,0.f };
	int controller1Index{ 0 };
	int controller2Index{ 1 };
	float controlSpeed{ 100.f };
	const int pacmanSizePx{ 50 };
	auto& inputInstance = dae::Input::GetInstance();

	auto pacman = std::make_shared<dae::GameObject>();
	auto renderComp = pacman->AddComponent<dae::RenderComponent>("PacMan.png");
	renderComp->SetWidthAndHeight(pacmanSizePx, pacmanSizePx);
	auto transform = pacman->AddComponent<dae::Transform>();
	transform->SetLocalPosition({ 125, 275,0 });
	transform->SetSpeedForMovement(controlSpeed);


	controlSpeed *= 2.f;
	auto pacman2 = std::make_shared<dae::GameObject>();
	auto renderComp2 = pacman2->AddComponent<dae::RenderComponent>("MsPacMan.png");
	auto transform2 = pacman2->AddComponent<dae::Transform>();
	transform2->SetLocalPosition({ 475, 275,0 });
	transform2->SetSpeedForMovement(controlSpeed);
	renderComp2->SetWidthAndHeight(pacmanSizePx, pacmanSizePx);
	

	dae::MoveCommand* moveCommandUp = new dae::MoveCommand{ pacman.get(), up };
	dae::MoveCommand* moveCommandDown = new dae::MoveCommand{ pacman.get(), down };
	dae::MoveCommand* moveCommandLeft = new dae::MoveCommand{ pacman.get(), left };
	dae::MoveCommand* moveCommandRight = new dae::MoveCommand{ pacman.get(), right };
	auto controllerButton = dae::XboxController::ControllerButton::DPadDown;
	auto keyState = dae::KeyState::pressed;
	auto keyboardKey = SDL_SCANCODE_S;

	inputInstance.AddController(controller1Index);
	inputInstance.BindButtonsToCommand(controller1Index, controllerButton,keyboardKey,keyState, moveCommandDown);
	controllerButton = dae::XboxController::ControllerButton::DPadUp;
	keyboardKey = SDL_SCANCODE_W;
	inputInstance.BindButtonsToCommand(controller1Index, controllerButton,keyboardKey,keyState, moveCommandUp);
	controllerButton = dae::XboxController::ControllerButton::DPadLeft;
	keyboardKey = SDL_SCANCODE_A;
	inputInstance.BindButtonsToCommand(controller1Index, controllerButton, keyboardKey,keyState, moveCommandLeft);
	controllerButton = dae::XboxController::ControllerButton::DPadRight;
	keyboardKey = SDL_SCANCODE_D;
	inputInstance.BindButtonsToCommand(controller1Index, controllerButton, keyboardKey,keyState, moveCommandRight);
	scene.Add(pacman);

	dae::MoveCommand* moveCommandJoystick = new dae::MoveCommand{ pacman.get() };
	inputInstance.BindJoystickToCommand(controller1Index, moveCommandJoystick);

	moveCommandUp = new dae::MoveCommand{ pacman2.get(), up };
	moveCommandDown = new dae::MoveCommand{ pacman2.get(), down };
	moveCommandLeft = new dae::MoveCommand{ pacman2.get(), left };
	moveCommandRight = new dae::MoveCommand{ pacman2.get(), right };
	controllerButton = dae::XboxController::ControllerButton::DPadDown;

	keyboardKey = SDL_SCANCODE_DOWN;
	inputInstance.AddController(controller2Index);
	inputInstance.BindButtonsToCommand(controller2Index, controllerButton,keyboardKey,keyState, moveCommandDown);
	keyboardKey = SDL_SCANCODE_UP;
	controllerButton = dae::XboxController::ControllerButton::DPadUp;
	inputInstance.BindButtonsToCommand(controller2Index, controllerButton, keyboardKey,keyState,moveCommandUp);
	keyboardKey = SDL_SCANCODE_LEFT;
	controllerButton = dae::XboxController::ControllerButton::DPadLeft;
	inputInstance.BindButtonsToCommand(controller2Index, controllerButton,keyboardKey,keyState, moveCommandLeft);
	controllerButton = dae::XboxController::ControllerButton::DPadRight;
	keyboardKey = SDL_SCANCODE_RIGHT;
	inputInstance.BindButtonsToCommand(controller2Index, controllerButton,keyboardKey,keyState, moveCommandRight);
	scene.Add(pacman2);

	moveCommandJoystick = new dae::MoveCommand{ pacman2.get() };
	inputInstance.BindJoystickToCommand(controller2Index, moveCommandJoystick);

	auto imGuiSpeedSliders = std::make_shared<dae::GameObject>();
	std::string title{ "Pacman movement test" };
	auto speedSlider = imGuiSpeedSliders->AddComponent<dae::ImGuiSpeedSliderComp>(title);
	speedSlider->AddTransform(transform);
	speedSlider->AddTransform(transform2);
	scene.Add(imGuiSpeedSliders);
}

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>("background.tga");
	auto transform = go->AddComponent<dae::Transform>();
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>("logo.tga");
	transform = go->AddComponent<dae::Transform>();
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
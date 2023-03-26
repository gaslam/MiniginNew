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
#include "InputManager.h"

void prepareControlsWithGameObjects(dae::Scene& scene) {
	glm::vec3 up = { 0.f,-1.f,0.f };
	glm::vec3 down = { 0.f,1.f,0.f };
	glm::vec3 right = { 1.f,0.f,0.f };
	glm::vec3 left = { -1.f,0.f,0.f };
	int controller1Index{ 0 };
	int controller2Index{ 1 };
	auto pacman = std::make_shared<dae::GameObject>();
	pacman->AddComponent<dae::RenderComponent>("pacman.png");
	auto transform = pacman->AddComponent<dae::Transform>();
	transform->SetLocalPosition({ 216, 180,0 });

	auto pacman2 = std::make_shared<dae::GameObject>();
	pacman2->AddComponent<dae::RenderComponent>("pacman.png");
	auto transform2 = pacman2->AddComponent<dae::Transform>();
	transform2->SetLocalPosition({ 266, 180,0 });

	dae::MoveCommand* moveCommandUp = new dae::MoveCommand{ pacman.get(), up };
	dae::MoveCommand* moveCommandDown = new dae::MoveCommand{ pacman.get(), down };
	dae::MoveCommand* moveCommandLeft = new dae::MoveCommand{ pacman.get(), left };
	dae::MoveCommand* moveCommandRight = new dae::MoveCommand{ pacman.get(), right };
	auto controllerButton = dae::XboxController::ControllerButton::DPadDown;

	dae::InputManager::GetInstance().AddController(controller1Index);
	dae::InputManager::GetInstance().BindControllerToCommand(controller1Index, controllerButton, moveCommandDown);
	controllerButton = dae::XboxController::ControllerButton::DPadUp;
	dae::InputManager::GetInstance().BindControllerToCommand(controller1Index, controllerButton, moveCommandUp);
	controllerButton = dae::XboxController::ControllerButton::DPadLeft;
	dae::InputManager::GetInstance().BindControllerToCommand(controller1Index, controllerButton, moveCommandLeft);
	controllerButton = dae::XboxController::ControllerButton::DPadRight;
	dae::InputManager::GetInstance().BindControllerToCommand(controller1Index, controllerButton, moveCommandRight);
	scene.Add(pacman);

	dae::MoveCommand* moveCommandJoystick = new dae::MoveCommand{ pacman.get() };
	dae::InputManager::GetInstance().BindJoystickToCommand(controller1Index, moveCommandJoystick);

	moveCommandUp = new dae::MoveCommand{ pacman2.get(), up };
	moveCommandDown = new dae::MoveCommand{ pacman2.get(), down };
	moveCommandLeft = new dae::MoveCommand{ pacman2.get(), left };
	moveCommandRight = new dae::MoveCommand{ pacman2.get(), right };
	controllerButton = dae::XboxController::ControllerButton::DPadDown;

	dae::InputManager::GetInstance().AddController(controller2Index);
	dae::InputManager::GetInstance().BindControllerToCommand(controller2Index, controllerButton, moveCommandDown);
	controllerButton = dae::XboxController::ControllerButton::DPadUp;
	dae::InputManager::GetInstance().BindControllerToCommand(controller2Index, controllerButton, moveCommandUp);
	controllerButton = dae::XboxController::ControllerButton::DPadLeft;
	dae::InputManager::GetInstance().BindControllerToCommand(controller2Index, controllerButton, moveCommandLeft);
	controllerButton = dae::XboxController::ControllerButton::DPadRight;
	dae::InputManager::GetInstance().BindControllerToCommand(controller2Index, controllerButton, moveCommandRight);
	scene.Add(pacman2);

	moveCommandJoystick = new dae::MoveCommand{ pacman2.get() };
	dae::InputManager::GetInstance().BindJoystickToCommand(controller2Index, moveCommandJoystick);
}

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	dae::InputManager::GetInstance();

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
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}
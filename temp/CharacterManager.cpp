#include "CharacterManager.h"

#include <SDL_scancode.h>

#include "Components/CharacterComponent.h"
#include <Transform.h>

#include "GameObject.h"
#include "Input.h"
#include "MoveCommand.h"
#include "SetCharacterStateToIdleCommand.h"
#include "Shape.h"
#include "Components/AnimationComponent.h"
#include "Components/RigidBodyComponent.h"

std::shared_ptr<dae::GameObject> dae::CharacterManager::InitPlayer()
{
	auto chef = std::make_shared<dae::GameObject>();
	auto transform = chef->AddComponent<dae::Transform>();
	transform->SetLocalPosition({ 100.f,100.f });
	transform->SetSpeedForMovement(50.f);
	float frameSec = 0.05f;
	float scale = 1.5f;
	int rows = 2;
	int cols = 9;
	int startRow = 0;
	int startCol = 1;
	int count = 1;
	dae::AnimationComponent* animComp = chef->AddComponent<dae::AnimationComponent>("Characters/ChefPeter.png", frameSec, scale, rows, cols, startRow, startCol, count, true);
	auto characterComp = chef->AddComponent<dae::CharacterComponent>(animComp);

	auto rigidBodyComp = chef->AddComponent<dae::RigidBodyComponent>(transform);
	auto rect = animComp->GetCell();
	dae::RectangleShape* shape = new dae::RectangleShape{ rect };
	rigidBodyComp->SetShape(shape);


	glm::vec3 up = { 0.f,-1.f,0.f };
	glm::vec3 down = { 0.f,1.f,0.f };
	glm::vec3 right = { 1.f,0.f,0.f };
	glm::vec3 left = { -1.f,0.f,0.f };
	int controller1Index{ 0 };
	auto& inputInstance = dae::Input::GetInstance();


	dae::MoveCommand* moveCommandUp = new dae::MoveCommand{ chef.get(), up };
	dae::MoveCommand* moveCommandDown = new dae::MoveCommand{ chef.get(), down };
	dae::MoveCommand* moveCommandLeft = new dae::MoveCommand{ chef.get(), left };
	dae::MoveCommand* moveCommandRight = new dae::MoveCommand{ chef.get(), right };
	dae::SetCharacterStateToIdleCommand* setToIdleCommand = new dae::SetCharacterStateToIdleCommand{ chef.get() };
	auto idleCommandKeyState = dae::KeyState::up;
	auto controllerButton = dae::XboxController::ControllerButton::DPadDown;
	auto keyState = dae::KeyState::pressed;
	auto keyboardKey = SDL_SCANCODE_S;
	dae::AnimationItem item{};

	item.startCol = 0;
	item.startRow = 0;
	item.startCol = 1;
	auto characterState = dae::CharacterComponent::CharacterState::idle;
	characterComp->AddAnimation(item, characterState);

	item.startCol = 6;
	item.startRow = 0;
	item.count = 3;
	item.isRepeatable = true;
	characterState = dae::CharacterComponent::CharacterState::moveDown;
	characterComp->AddAnimation(item, characterState);

	inputInstance.AddController(controller1Index);
	inputInstance.BindButtonsToCommand(controller1Index, controllerButton, keyboardKey, keyState, moveCommandDown);
	inputInstance.BindButtonsToCommand(controller1Index, controllerButton, keyboardKey, idleCommandKeyState, setToIdleCommand);
	controllerButton = dae::XboxController::ControllerButton::DPadUp;
	item.startCol = 0;
	item.startRow = 0;
	item.count = 3;
	item.isRepeatable = true;
	characterState = dae::CharacterComponent::CharacterState::moveUp;
	characterComp->AddAnimation(item, characterState);
	keyboardKey = SDL_SCANCODE_W;
	inputInstance.BindButtonsToCommand(controller1Index, controllerButton, keyboardKey, keyState, moveCommandUp);
	inputInstance.BindButtonsToCommand(controller1Index, controllerButton, keyboardKey, idleCommandKeyState, setToIdleCommand);
	controllerButton = dae::XboxController::ControllerButton::DPadLeft;
	item.startCol = 3;
	item.startRow = 0;
	item.count = 3;
	item.isRepeatable = true;
	characterState = dae::CharacterComponent::CharacterState::moveLeft;
	characterComp->AddAnimation(item, characterState);
	keyboardKey = SDL_SCANCODE_A;
	inputInstance.BindButtonsToCommand(controller1Index, controllerButton, keyboardKey, keyState, moveCommandLeft);
	inputInstance.BindButtonsToCommand(controller1Index, controllerButton, keyboardKey, idleCommandKeyState, setToIdleCommand);
	controllerButton = dae::XboxController::ControllerButton::DPadRight;
	keyboardKey = SDL_SCANCODE_D;
	item.startCol = 3;
	item.startRow = 0;
	item.count = 3;
	item.isRepeatable = true;
	item.isXflipped = true;
	characterState = dae::CharacterComponent::CharacterState::moveRight;
	characterComp->AddAnimation(item, characterState);
	inputInstance.BindButtonsToCommand(controller1Index, controllerButton, keyboardKey, keyState, moveCommandRight);
	inputInstance.BindButtonsToCommand(controller1Index, controllerButton, keyboardKey, idleCommandKeyState, setToIdleCommand);

	return chef;
}

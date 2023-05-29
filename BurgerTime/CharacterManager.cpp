#include "CharacterManager.h"

#include <SDL_scancode.h>

#include "Components/CharacterComponent.h"
#include <Transform.h>

#include "GameObject.h"
#include "InputManager.h"
#include "KeyStructs.h"
#include "Locator.h"
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
	auto sound = Locator::GetAudio();
	auto characterComp = chef->AddComponent<dae::CharacterComponent>(animComp,sound);

	auto rigidBodyComp = chef->AddComponent<dae::RigidBodyComponent>(transform);
	auto rect = animComp->GetCell();
	dae::RectangleShape* shape = new dae::RectangleShape{ rect };
	rigidBodyComp->SetShape(shape);


	glm::vec2 up = { 0.f,-1.f };
	glm::vec2 down = { 0.f,1.f };
	glm::vec2 right = { 1.f,0.f };
	glm::vec2 left = { -1.f,0.f };
	int controller1Index{ 0 };
	auto& inputInstance = dae::InputManager::GetInstance();

	//dae::MoveCommand* moveCommandUp = new dae::MoveCommand{ chef.get(), up };
	//dae::MoveCommand* moveCommandDown = new dae::MoveCommand{ chef.get(), down };
	//dae::MoveCommand* moveCommandRight = new dae::MoveCommand{ chef.get(), right };
	auto controllerButton = dae::XboxController::ControllerButton::DPadDown;
	auto keyState = KeyState::pressed;
	auto keyboardKey = SDL_SCANCODE_S;
	std::string inputAction{"down"};
	dae::AnimationItem item{};

	item.startRow = 0;
	item.startCol = 1;
	auto characterState = dae::CharacterComponent::State::idle;
	characterComp->AddAnimation(item, characterState);

	item.startCol = 6;
	item.startRow = 0;
	item.count = 3;
	item.isRepeatable = true;
	characterState = dae::CharacterComponent::State::moveDown;
	characterComp->AddAnimation(item, characterState);

	inputInstance.BindButtonsToInput(inputAction, controller1Index, controllerButton, keyboardKey, keyState);
	controllerButton = dae::XboxController::ControllerButton::DPadUp;
	item.startCol = 0;
	item.startRow = 0;
	item.count = 3;
	item.isRepeatable = true;
	characterState = dae::CharacterComponent::State::moveUp;
	characterComp->AddAnimation(item, characterState);
	keyboardKey = SDL_SCANCODE_W;
	inputAction = "up";
	inputInstance.BindButtonsToInput(inputAction, controller1Index, controllerButton, keyboardKey, keyState);
	controllerButton = dae::XboxController::ControllerButton::DPadLeft;
	item.startCol = 3;
	item.startRow = 0;
	item.count = 3;
	item.isRepeatable = true;
	characterState = dae::CharacterComponent::State::moveLeft;
	characterComp->AddAnimation(item, characterState);
	keyboardKey = SDL_SCANCODE_A;
	inputAction = "left";
	inputInstance.BindButtonsToInput(inputAction, controller1Index, controllerButton, keyboardKey, keyState);
	controllerButton = dae::XboxController::ControllerButton::DPadRight;
	keyboardKey = SDL_SCANCODE_D;
	item.startCol = 3;
	item.startRow = 0;
	item.count = 3;
	item.isRepeatable = true;
	item.isXflipped = true;
	characterState = dae::CharacterComponent::State::moveRight;
	characterComp->AddAnimation(item, characterState);
	inputAction = "right";
	inputInstance.BindButtonsToInput(inputAction, controller1Index, controllerButton, keyboardKey, keyState);

	m_Player =chef.get();

	return chef;
}

std::vector<dae::GameObject*> dae::CharacterManager::GetCharacters()
{
	auto characters = m_Enemies;
	MG_ASSERT_WARNING(m_Player != nullptr,"Cannot find or control player!!");
	if(m_Player != nullptr)
	{
		characters.emplace_back(m_Player);
	}
	return characters;
}

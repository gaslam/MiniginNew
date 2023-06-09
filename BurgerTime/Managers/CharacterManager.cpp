#include "CharacterManager.h"

#include <SDL_scancode.h>

#include "../Components/CharacterComponent.h"
#include <Misc/Transform.h>

#include <Misc/GameObject.h>
#include <Managers/InputManager.h>
#include <Controls/KeyStructs.h>
#include <Misc/Locator.h>
#include <Misc/Scene.h>
#include <Misc/Shape.h>
#include "../Components/AnimationComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpawnerComponent.h"
#include "../Components/MrEggComponent.h"

using namespace dae;

std::shared_ptr<GameObject> CharacterManager::InitPlayer()
{
	auto chef = std::make_shared<GameObject>();
	auto transform = chef->AddComponent<Transform>();
	transform->SetSpeedForMovement(50.f);
	float frameSec = 0.05f;
	float scale = 1.5f;
	int rows = 2;
	int cols = 9;
	int startRow = 0;
	int startCol = 1;
	int count = 1;
	AnimationComponent* animComp = chef->AddComponent<AnimationComponent>("Characters/ChefPeter.png", frameSec, scale, rows, cols, startRow, startCol, count, true);
	const auto characterComp = chef->AddComponent<CharacterComponent>(animComp);

	const auto rigidBodyComp = chef->AddComponent<RigidBodyComponent>(transform);
	const auto rect = animComp->GetCell();
	auto* shape = new RectangleShape{ rect };
	rigidBodyComp->SetShape(shape);


	constexpr int controller1Index{ 0 };
	auto& inputInstance = InputManager::GetInstance();

	auto controllerButton = XboxController::ControllerButton::DPadDown;
	auto keyState = KeyState::pressed;
	auto keyboardKey = SDL_SCANCODE_S;
	std::string inputAction{"down"};
	AnimationItem item{};

	item.startRow = 0;
	item.startCol = 1;
	auto characterState = CharacterComponent::State::idle;
	characterComp->AddAnimation(item, characterState);

	item.startCol = 6;
	item.startRow = 0;
	item.count = 3;
	item.isRepeatable = true;
	characterState = CharacterComponent::State::moveDown;
	characterComp->AddAnimation(item, characterState);

	inputInstance.BindButtonsToInput(inputAction, controller1Index, controllerButton, keyboardKey, keyState);
	controllerButton = XboxController::ControllerButton::DPadUp;
	item.startCol = 0;
	item.startRow = 0;
	item.count = 3;
	item.isRepeatable = true;
	characterState = CharacterComponent::State::moveUp;
	characterComp->AddAnimation(item, characterState);
	keyboardKey = SDL_SCANCODE_W;
	inputAction = "up";
	inputInstance.BindButtonsToInput(inputAction, controller1Index, controllerButton, keyboardKey, keyState);
	controllerButton = XboxController::ControllerButton::DPadLeft;
	item.startCol = 3;
	item.startRow = 0;
	item.count = 3;
	item.isRepeatable = true;
	characterState = CharacterComponent::State::moveLeft;
	characterComp->AddAnimation(item, characterState);
	keyboardKey = SDL_SCANCODE_A;
	inputAction = "left";
	inputInstance.BindButtonsToInput(inputAction, controller1Index, controllerButton, keyboardKey, keyState);
	controllerButton = XboxController::ControllerButton::DPadRight;
	keyboardKey = SDL_SCANCODE_D;
	item.startCol = 3;
	item.startRow = 0;
	item.count = 3;
	item.isRepeatable = true;
	item.isXflipped = true;
	characterState = CharacterComponent::State::moveRight;
	characterComp->AddAnimation(item, characterState);
	inputAction = "right";
	inputInstance.BindButtonsToInput(inputAction, controller1Index, controllerButton, keyboardKey, keyState);

	m_Player =chef.get();

	return chef;
}

void CharacterManager::InitEnemies(Scene& scene,glm::vec2& gridStartPos,int backgroundWidth, int backgroundHeight, float /*scale*/) const
{
	const auto pGameObject{ std::make_shared<GameObject>() };
	const int rowsAndColumns{ 25 };
	const auto pGrid{ pGameObject->AddComponent<GridComponent>(gridStartPos,backgroundWidth,backgroundHeight,rowsAndColumns,rowsAndColumns) };
	const glm::vec2 spawnPos{50, 50};
	const auto pSpawner{ pGameObject->AddComponent<SpawnerComponent<MrEggComponent>>(spawnPos) };
	const auto pEnemy{ std::make_shared<GameObject>() };
	const auto pEnemyComp{ pSpawner->Spawn(pEnemy.get()) };
	scene.Add(pGameObject);
	scene.Add(pEnemy);
	pEnemyComp->SetGrid(pGrid);
}

std::vector<GameObject*> CharacterManager::GetCharacters()
{
	auto characters = m_Enemies;
	MG_ASSERT_WARNING(m_Player != nullptr,"Cannot find or control player!!");
	if(m_Player != nullptr)
	{
		characters.emplace_back(m_Player);
	}
	return characters;
}

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
#include "RenderComponent.h"
#include "Input.h"
#include <string>
#include "Renderer.h"
#include "AnimationComponent.h"
#include "CharacterComponent.h"
#include "MrEggComponent.h"
#include "RigidBodyComponent.h"
#include "SpawnerComponent.h"
#include "Utils.h"


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
	glm::vec3 worldPos = { windowSize.x / 2.f - backgroundWidth / 2.f,windowSize.y - backgroundHeight, 0.f };
	transform->SetWorldPosition(worldPos);
	std::string file{ "../Data/LevelData/Stage" + stageIdStr + "Data.btf" };
	Utils::ReadLevelData(file, &scene,worldPos,worldScale);
	auto chef = std::make_shared<dae::GameObject>();
	transform = chef->AddComponent<dae::Transform>();
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
	scene.Add(chef);

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
	dae::SetChacterToStateIdleCommand* setToIdleCommand = new dae::SetChacterToStateIdleCommand{ chef.get() };
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

	go = std::make_shared<dae::GameObject>();
	glm::vec2 spawnPos = glm::vec2{ 50, 50 };
	go->AddComponent<dae::SpawnerComponent<dae::MrEggComponent>>(spawnPos);
	scene.Add(go);
}

void load()
{
	MG_ASSERT_INFO("Starting load Process!!");
	MG_ASSERT_INFO("Adding scene 1!!");
	AddScene(6);
}

int main(int, char*[]) {
	MG_ASSERT_INFO("Starting program!!");
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}
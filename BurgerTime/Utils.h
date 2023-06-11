#pragma once
#include <fstream>
#include "Components/RigidBodyComponent.h"
#include "Components/RenderComponent.h"
#include "Components/LadderComponent.h"
#include "Components/PlatformComponent.h"
#include "Components/BurgerComponent.h"
#include "Observers/BurgerObserver.h"
#include "Observers/PlatformObserver.h"
#include <Misc/Scene.h>
#include <Misc/GameObject.h>

#include "Components/ImGuiSoundRenderer.h"
#include "Managers/CharacterManager.h"
#include "Managers/InputManager.h"
#include "Components/SceneComponent.h"
#include "Commands/SceneSwitchCommand.h"

using namespace dae;
namespace Utils
{
	template <typename T>
	std::enable_if_t<std::is_base_of_v<dae::Component, T>, std::shared_ptr<GameObject>>
	 GenerateObjectWithComponent(std::ifstream& stream, glm::vec2& worldPos, float scale)
	{
		float x{}, y{};
		float width{}, height{};
		auto platform{ std::make_shared<GameObject>() };
		auto pTransform{ platform->AddComponent<Transform>() };
		platform->AddComponent<RigidBodyComponent>(pTransform);


		stream >> x;
		stream >> y;
		stream >> height;
		stream >> width;

		glm::vec2 pos{ x,y };

		width *= scale;
		height *= scale;
		pos *= scale;
		pos += worldPos;
		pTransform->SetWorldPosition(pos);
		constexpr glm::ivec2 rectStartPos{ 0,0 };
		auto pShape{ new RectangleShape{ rectStartPos,static_cast<int>(width),static_cast<int>(height) } };
		platform->AddComponent<T>(pShape);
		return platform;
	}

	template <typename T>
	std::enable_if_t<std::is_base_of_v<dae::Component, T>, std::shared_ptr<GameObject>>
		GenerateObjectWithComponentAndTexture(std::ifstream& stream, glm::vec2& worldPos, float scale)
	{
		float x{}, y{};
		auto platform{ std::make_shared<GameObject>() };
		auto pTransform{ platform->AddComponent<Transform>() };
		std::string texturePath{};
		platform->AddComponent<RigidBodyComponent>(pTransform);

		stream >> texturePath;
		stream >> x;
		stream >> y;

		glm::vec2 pos{ x, y };
		pos *= scale;
		pos += worldPos;
		pTransform->SetWorldPosition(pos);
		platform->AddComponent<T>(texturePath,pos,scale);
		return platform;
	}

	inline void ReadLevelData(std::string& file, Scene* scene, glm::vec2& worldPos, float scale)
	{
		std::ifstream stream{ file, std::ios_base::in };
		std::vector<std::shared_ptr<Observer>> platformObservers{};
		std::vector<std::shared_ptr<BurgerObserver>> burgerObservers{};
		std::vector<BurgerComponent*> burgerComponents{};
		while (stream)
		{
			std::string line{};
			std::getline(stream, line);

			if (line == "Platform:")
			{
				const auto platform{ GenerateObjectWithComponent<PlatformComponent>(stream, worldPos, scale) };
				auto pComponent{ platform->GetComponent<PlatformComponent>() };
				platformObservers.emplace_back(std::make_shared<PlatformObserver>(pComponent));
				scene->Add(platform);
			}

			if (line == "BurgerDropoff:")
			{
				//const auto dropoff{GenerateObjectWithComponent(stream, worldPos, scale);
				//scene->Add(dropoff);
			}

			if (line == "Ladder:")
			{
				const auto dropoff{ GenerateObjectWithComponent<LadderComponent>(stream, worldPos, scale) };
				scene->Add(dropoff);
			}
			if (line == "Burger:")
			{
				const auto burger{ GenerateObjectWithComponentAndTexture<BurgerComponent>(stream, worldPos, scale) };
				auto pComponent{ burger->GetComponent<BurgerComponent>() };
				burgerObservers.emplace_back(std::make_shared<BurgerObserver>(pComponent));
				burgerComponents.emplace_back(burger->GetComponent<BurgerComponent>());
				scene->Add(burger);
			}
		}

		for(const auto burger: burgerComponents)
		{
			for(const auto observer: platformObservers)
			{
				burger->AddObserver(observer);
			}
		}

		for(size_t i{}; i < burgerObservers.size(); i += 4)
		{
			size_t possibleNextObservers{ i + 4 };
			if(possibleNextObservers> burgerObservers.size())
			{
				possibleNextObservers = burgerObservers.size();
			}
			std::vector<std::shared_ptr<BurgerObserver>> range{ &burgerObservers[0] + i, &burgerObservers[0]+ possibleNextObservers };
			for(const auto burger: burgerComponents)
			{
				for(const auto observer: range)
				{
					if(observer->GetComponent() != burger)
					{
						burger->AddObserver(observer);
					}
				}
			}
		}
	}

	inline void AddScene(int stageId, std::shared_ptr<GameObject> player)
	{
		const std::string stageIdStr{ std::to_string(stageId) };
		auto& scene{ SceneManager::GetInstance().CreateScene("Stage " + stageIdStr) };

		const auto go{ std::make_shared<GameObject>() };
		const auto render{ go->AddComponent<RenderComponent>("Backgrounds/Stage_" + stageIdStr + ".png") };
		const float worldScale{ 3.f };
		render->SetScale(worldScale);
		const auto transform{ go->AddComponent<Transform>() };
		const glm::ivec2 windowSize{Renderer::GetInstance().GetWindowWidthAndHeight()};
		const float backgroundWidth{ render->GetWidthScaled() };
		const float backgroundHeight{ render->GetHeightScaled() };
		scene.Add(go);

		const auto imGuiSoundRenderer{ std::make_shared<GameObject>() };
		imGuiSoundRenderer->AddComponent<ImGuiSoundRenderer>();
		scene.Add(imGuiSoundRenderer);

		const auto sceneObj{ std::make_shared<GameObject>() };
		sceneObj->AddComponent<SceneComponent>();
		scene.Add(sceneObj);

		glm::vec2 worldPos{ windowSize.x / 2.f - backgroundWidth / 2.f, windowSize.y - backgroundHeight };
		transform->SetWorldPosition(worldPos);
		std::string file{ "../Data/LevelData/Stage" + stageIdStr + "Data.btf" };
		ReadLevelData(file, &scene, worldPos, worldScale);
		scene.Add(player);

	}

	inline void AddScenes()
	{
		auto player{ CharacterManager::GetInstance().InitPlayer() };
		AddScene(1,player);
		AddScene(2,player);
		AddScene(3,player);

		SceneManager::GetInstance().SetScene(0);
		bool moveToNext{false};
		constexpr int controllerIndex{ 0 };
		auto controllerButton{ XboxController::ControllerButton::LeftShoulder };
		SDL_Scancode keyboardButton{ SDL_SCANCODE_F3 };
		auto keyState{ KeyState::down };
		InputManager::GetInstance().BindButtonsToCommand(controllerIndex, controllerButton, keyboardButton, keyState, new SceneSwitchCommand{ moveToNext });
		moveToNext = true;
		controllerButton = XboxController::ControllerButton::RightShoulder;
		keyboardButton = SDL_SCANCODE_F4;
		InputManager::GetInstance().BindButtonsToCommand(controllerIndex, controllerButton, keyboardButton, keyState, new dae::SceneSwitchCommand{ moveToNext });
	}
}

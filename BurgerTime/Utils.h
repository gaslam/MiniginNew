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

namespace Utils
{
	template <typename T>
	std::enable_if_t<std::is_base_of_v<dae::Component, T>, std::shared_ptr<dae::GameObject>>
	 GenerateObjectWithComponent(std::ifstream& stream, glm::vec2& worldPos, float scale)
	{
		float x{}, y{};
		float width{}, height{};
		auto platform = std::make_shared<dae::GameObject>();
		auto pTransform = platform->AddComponent<dae::Transform>();
		platform->AddComponent<dae::RigidBodyComponent>(pTransform);


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
		auto pShape = new dae::RectangleShape{ rectStartPos,static_cast<int>(width),static_cast<int>(height) };
		platform->AddComponent<T>(pShape);
		return platform;
	}

	template <typename T>
	std::enable_if_t<std::is_base_of_v<dae::Component, T>, std::shared_ptr<dae::GameObject>>
		GenerateObjectWithComponentAndTexture(std::ifstream& stream, glm::vec2& worldPos, float scale)
	{
		float x{}, y{};
		auto platform = std::make_shared<dae::GameObject>();
		auto pTransform = platform->AddComponent<dae::Transform>();
		std::string texturePath{};
		platform->AddComponent<dae::RigidBodyComponent>(pTransform);

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

	inline void ReadLevelData(std::string& file, dae::Scene* scene, glm::vec2& worldPos, float scale)
	{
		std::ifstream stream{ file, std::ios_base::in };
		std::vector<std::shared_ptr<dae::Observer>> platformObservers{};
		std::vector<std::shared_ptr<dae::BurgerObserver>> burgerObservers{};
		std::vector<dae::BurgerComponent*> burgerComponents{};
		while (stream)
		{
			std::string line{};
			std::getline(stream, line);

			if (line == "Platform:")
			{
				const auto platform = GenerateObjectWithComponent<dae::PlatformComponent>(stream, worldPos, scale);
				auto pComponent{ platform->GetComponent<dae::PlatformComponent>() };
				platformObservers.emplace_back(std::make_shared<dae::PlatformObserver>(pComponent));
				scene->Add(platform);
			}

			if (line == "BurgerDropoff:")
			{
				//const auto dropoff = GenerateObjectWithComponent(stream, worldPos, scale);
				//scene->Add(dropoff);
			}

			if (line == "Ladder:")
			{
				const auto dropoff = GenerateObjectWithComponent<dae::LadderComponent>(stream, worldPos, scale);
				scene->Add(dropoff);
			}
			if (line == "Burger:")
			{
				const auto burger = GenerateObjectWithComponentAndTexture<dae::BurgerComponent>(stream, worldPos, scale);
				auto pComponent{ burger->GetComponent<dae::BurgerComponent>() };
				burgerObservers.emplace_back(std::make_shared<dae::BurgerObserver>(pComponent));
				burgerComponents.emplace_back(burger->GetComponent<dae::BurgerComponent>());
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
			std::vector<std::shared_ptr<dae::BurgerObserver>> range{ &burgerObservers[0] + i, &burgerObservers[0]+ possibleNextObservers };
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

	inline void AddScene(int stageId, std::shared_ptr<dae::GameObject> player)
	{
		std::string stageIdStr{ std::to_string(stageId) };
		auto& scene = dae::SceneManager::GetInstance().CreateScene("Stage " + stageIdStr);

		auto go = std::make_shared<dae::GameObject>();
		auto render = go->AddComponent<dae::RenderComponent>("Backgrounds/Stage_" + stageIdStr + ".png");
		const float worldScale{ 3.f };
		render->SetScale(worldScale);
		auto transform = go->AddComponent<dae::Transform>();
		glm::ivec2 windowSize = dae::Renderer::GetInstance().GetWindowWidthAndHeight();
		const float backgroundWidth = render->GetWidthScaled();
		const float backgroundHeight = render->GetHeightScaled();
		scene.Add(go);

		auto imGuiSoundRenderer{ std::make_shared<dae::GameObject>() };
		imGuiSoundRenderer->AddComponent<dae::ImGuiSoundRenderer>();
		scene.Add(imGuiSoundRenderer);

		auto sceneObj{ std::make_shared<dae::GameObject>() };
		sceneObj->AddComponent<dae::SceneComponent>();
		scene.Add(sceneObj);

		glm::vec2 worldPos = { windowSize.x / 2.f - backgroundWidth / 2.f,windowSize.y - backgroundHeight };
		transform->SetWorldPosition(worldPos);
		std::string file{ "../Data/LevelData/Stage" + stageIdStr + "Data.btf" };
		ReadLevelData(file, &scene, worldPos, worldScale);
		scene.Add(player);

	}

	inline void AddScenes()
	{
		auto player{ dae::CharacterManager::GetInstance().InitPlayer() };
		AddScene(1,player);
		AddScene(2,player);
		AddScene(3,player);

		dae::SceneManager::GetInstance().SetScene(0);
		bool moveToNext{false};
		constexpr int controllerIndex{ 0 };
		auto controllerButton{ dae::XboxController::ControllerButton::LeftShoulder };
		SDL_Scancode keyboardButton{ SDL_SCANCODE_F3 };
		auto keyState{ KeyState::down };
		dae::InputManager::GetInstance().BindButtonsToCommand(controllerIndex, controllerButton, keyboardButton, keyState, new dae::SceneSwitchCommand{ moveToNext });
		moveToNext = true;
		controllerButton = dae::XboxController::ControllerButton::RightShoulder;
		keyboardButton = SDL_SCANCODE_F4;
		dae::InputManager::GetInstance().BindButtonsToCommand(controllerIndex, controllerButton, keyboardButton, keyState, new dae::SceneSwitchCommand{ moveToNext });
	}
}

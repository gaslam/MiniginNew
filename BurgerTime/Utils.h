#pragma once
#include <fstream>
#include "Components/RigidBodyComponent.h"
#include "GameObject.h"
#include "Components/LadderComponent.h"
#include "Components/PlatformComponent.h"
#include "Observers/PlatformObserver.h"

namespace Utils
{
	template <typename T>
	std::enable_if_t<std::is_base_of_v<dae::Component, T>, std::shared_ptr<dae::GameObject>>
	 GenerateRigidBodyComp(std::ifstream& stream, glm::vec2& worldPos, float scale)
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
		auto rectStartPos = glm::ivec2{ 0,0 };
		auto pShape = new dae::RectangleShape{ rectStartPos,static_cast<int>(width),static_cast<int>(height) };
		platform->AddComponent<T>(pShape);
		return platform;
	}

	template <typename T>
	std::enable_if_t<std::is_base_of_v<dae::Component, T>, std::shared_ptr<dae::GameObject>>
		GenerateRigidBodyCompWithTexture(std::ifstream& stream, glm::vec2& worldPos, float scale)
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
		std::vector<dae::BurgerComponent*> burgerComponents{};
		while (stream)
		{
			std::string line{};
			std::getline(stream, line);

			if (line == "Platform:")
			{
				const auto platform = GenerateRigidBodyComp<dae::PlatformComponent>(stream, worldPos, scale);
				auto pComponent{ platform->GetComponent<dae::RigidBodyComponent>() };
				platformObservers.emplace_back(std::make_shared<dae::PlatformObserver>(pComponent));
				scene->Add(platform);
			}

			if (line == "BurgerDropoff:")
			{
				//const auto dropoff = GenerateRigidBodyComp(stream, worldPos, scale);
				//scene->Add(dropoff);
			}

			if (line == "Ladder:")
			{
				const auto dropoff = GenerateRigidBodyComp<dae::LadderComponent>(stream, worldPos, scale);
				scene->Add(dropoff);
			}
			if (line == "Burger:")
			{
				const auto burger = GenerateRigidBodyCompWithTexture<dae::BurgerComponent>(stream, worldPos, scale);
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
	}
}

#include <fstream>

#include "Scene.h"

namespace DataUtils
{
	inline std::shared_ptr<dae::GameObject> GenerateRigidBodyComp(std::ifstream& stream, glm::vec3& worldPos, float scale)
	{
		float x{}, y{};
		float width{}, height{};
		auto platform = std::make_shared<dae::GameObject>();
		auto pTransform = platform->AddComponent<dae::Transform>();
		auto rigidBody = platform->AddComponent<dae::RigidBodyComponent>(pTransform);
		auto rectStartPos = glm::ivec2{ 0,0 };

		stream >> x;
		stream >> y;
		stream >> height;
		stream >> width;

		glm::vec3 pos{ x,y,0.f };

		width *= scale;
		height *= scale;
		pos *= scale;
		pos += worldPos;
		pTransform->SetWorldPosition(pos);

		auto pShape = new RectangleShape{ rectStartPos,static_cast<int>(width),static_cast<int>(height) };
		rigidBody->SetShape(pShape);
		return platform;
	}

	inline void ReadLevelData(std::string& file, dae::Scene* scene, glm::vec3& worldPos, float scale)
	{
		std::ifstream stream{ file, std::ios_base::in };
		while(stream)
		{
			std::string line{};
			std::getline(stream, line);

			if(line == "Platform:")
			{
				const auto platform = GenerateRigidBodyComp(stream, worldPos, scale);
				scene->Add(platform);
			}

			if(line == "BurgerDropoff:")
			{
				const auto dropoff = GenerateRigidBodyComp(stream, worldPos, scale);
				scene->Add(dropoff);
			}

			if (line == "Ladder:")
			{
				const auto dropoff = GenerateRigidBodyComp(stream, worldPos, scale);
				scene->Add(dropoff);
			}
		}
	}
}

#include "MrEggComponent.h"

#include "AnimationComponent.h"
#include "GameObject.h"

dae::MrEggComponent::MrEggComponent(GameObject* pObject) : EnemyComponent(pObject)
{
	const std::string textureFile = "Characters/Mr.Egg.png";
	constexpr float frameSec = 0.05f;
	constexpr float scale = 1.5f;
	constexpr int rows = 2;
	constexpr int cols = 6;
	constexpr int startRow = 0;
	constexpr int startCol = 0;
	constexpr int count = 1;
	constexpr bool  canRepeat = true;
	pObject->AddComponent<AnimationComponent>(textureFile, frameSec, scale, rows, cols, startRow, startCol, count, canRepeat);
}

dae::EnemyComponent* dae::MrEggComponent::Clone(GameObject* object)
{
	if (object->IsComponentAdded<MrEggComponent>())
	{
		object->RemoveComponent<MrEggComponent>();
	}
	return object->AddComponent<MrEggComponent>();
}
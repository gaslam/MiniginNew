#include "MrHotdogComp.h"
#include "AnimationComponent.h"
#include <Misc/GameObject.h>

dae::MrHotdogComp::MrHotdogComp(GameObject* pObject) : EnemyComponent(pObject)
{
	const std::string textureFile = "Characters/Mr.Hotdog.png";
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

dae::EnemyComponent* dae::MrHotdogComp::Clone(GameObject* object)
{
	if (object->IsComponentAdded<MrHotdogComp>())
	{
		object->RemoveComponent<MrHotdogComp>();
	}
	return object->AddComponent<MrHotdogComp>();
}

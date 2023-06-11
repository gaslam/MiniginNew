#include "PlatformComponent.h"
#include "RenderComponent.h"
#include "RigidBodyComponent.h"
#include "CharacterComponent.h"
#include "../Managers/CharacterManager.h"
#include <algorithm>
#include <Misc/GameObject.h>

using namespace dae;
PlatformComponent::PlatformComponent(GameObject* pOwner, RectangleShape* pShape) : Component(pOwner)
{
	m_pRigidBodyComp = pOwner->AddComponent<RigidBodyComponent>(pOwner->GetComponent<Transform>());

	m_pRigidBodyComp->SetShape(pShape);

}


void PlatformComponent::Update(float)
{
	const auto characters = CharacterManager::GetInstance().GetCharacters();
	for (const auto character : characters)
	{
		const auto pTransform = character->GetComponent<Transform>();
		MG_ASSERT(pTransform != nullptr, "Cannot get Transform!!");
		const auto pCharacterComp = character->GetComponent<CharacterComponent>();
		MG_ASSERT(pCharacterComp != nullptr, "Cannot get CharacterComponent!!");
		const auto pRenderComp = character->GetComponent<RenderComponent>();
		MG_ASSERT(pRenderComp != nullptr, "Cannot get RenderComponent!!");

		const int frameWidth = static_cast<int>(pRenderComp->GetFrameWidthScaled());
		const int frameHeight = static_cast<int>(pRenderComp->GetFrameHeightScaled());
		auto point = pTransform->GetLocalPosition();
		auto pointVec2 = glm::ivec2{ point.x + frameWidth,point.y + frameHeight };
		bool canMove = m_pRigidBodyComp->IsPointInRect(pointVec2);
		auto points = m_pRigidBodyComp->GetShape()->GetPoints();
		glm::ivec2 minX = *std::min_element(points.begin(), points.end(), [](const glm::ivec2& point1, const glm::ivec2& point2)
			{
				return point1.x < point2.x;
			});

		auto maxX = *std::max_element(points.begin(), points.end(), [](const glm::ivec2& point1, const glm::ivec2& point2)
			{
				return point1.x < point2.x;
			});

		glm::ivec2 minY = *std::min_element(points.begin(), points.end(), [](glm::ivec2& point1, glm::ivec2& point2)
			{
				return point1.y < point2.y;
			});

		minX.x += 1;
		maxX.x -= 1;

		if (pointVec2.x >= maxX.x && canMove)
		{
			canMove = false;
			point.x -= 1;
			pTransform->SetLocalPosition(point);
		}
		if (pointVec2.x - frameWidth <= minX.x && canMove)
		{
			canMove = false;
			point.x += 1;
			pTransform->SetLocalPosition(point);
		}

		const auto characterState = pCharacterComp->GetState();
		const bool isMovingHorizontally{ characterState == CharacterComponent::moveRight || characterState == CharacterComponent::moveLeft };
		if (canMove && isMovingHorizontally)
		{
			auto characterNewPos = glm::vec3(point.x, minY.y - frameHeight, 0);
			pTransform->SetLocalPosition(characterNewPos);
		}

		pCharacterComp->SetMovementLeftRight(canMove);
	}
}
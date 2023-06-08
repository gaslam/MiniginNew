#include "LadderComponent.h"

#include "AnimationComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "RigidBodyComponent.h"
#include "Shape.h"
#include <algorithm>

#include "CharacterComponent.h"
#include <Transform.h>
#include "Logger.h"
#include "../CharacterManager.h"

using namespace dae;
LadderComponent::LadderComponent(GameObject* pOwner, RectangleShape* pShape) : Component(pOwner)
{
	m_pRigidBodyComp = pOwner->AddComponent<RigidBodyComponent>(pOwner->GetComponent<Transform>());
	MG_ASSERT(m_pRigidBodyComp != nullptr,"Cannot add RigidBodyComp!!")
	m_pRigidBodyComp->SetShape(pShape);

}


void LadderComponent::Update(float)
{
	const auto characters = CharacterManager::GetInstance().GetCharacters();
	for(const auto character: characters )
	{
		const auto pTransform = character->GetComponent<Transform>();
		MG_ASSERT(pTransform != nullptr,"Cannot get Transform!!");
		const auto pCharacterComp = character->GetComponent<CharacterComponent>();
		MG_ASSERT(pCharacterComp != nullptr,"Cannot get CharacterComponent!!");
		const auto pRenderComp = character->GetComponent<RenderComponent>();
		MG_ASSERT(pRenderComp != nullptr, "Cannot get RenderComponent!!");

		const float frameHeight = pRenderComp->GetFrameHeightScaled();
		auto point = pTransform->GetLocalPosition();
		auto pointVec2 = glm::ivec2{ point.x,point.y + frameHeight };
		bool canMove = m_pRigidBodyComp->IsPointInRect(pointVec2);
		auto points = m_pRigidBodyComp->GetShape()->GetPoints();
		glm::ivec2 minY = *std::min_element(points.begin(), points.end(), [](glm::ivec2& point1, glm::ivec2& point2)
			{
				return point1.y < point2.y;
			});

		glm::ivec2 minX = *std::min_element(points.begin(), points.end(), [](glm::ivec2& point1, glm::ivec2& point2)
			{
				return point1.x < point2.x;
			});

		glm::ivec2 maxY = *std::max_element(points.begin(), points.end(), [](glm::ivec2& point1, glm::ivec2& point2)
			{
				return point1.y < point2.y;
			});

		glm::ivec2 maxX = *std::max_element(points.begin(), points.end(), [](glm::ivec2& point1, glm::ivec2& point2)
			{
				return point1.x < point2.x;
			});

		minY.y += 1;
		maxY.y -= 1;

		if (pointVec2.y >= maxY.y && canMove)
		{
			canMove = false;
			point.y -= 1;
			pTransform->SetLocalPosition(point);
		}
		if (pointVec2.y <= minY.y && canMove)
		{
			canMove = false;
			point.y += 1;
			pTransform->SetLocalPosition(point);
		}

		const auto characterState = pCharacterComp->GetState();
		const bool isMovingVertically{ characterState == CharacterComponent::moveDown || characterState == CharacterComponent::moveUp };
		if (canMove && isMovingVertically)
		{
			float frameWidthDivBy2 = pRenderComp->GetFrameWidthScaled() / 2.f;
			//Starts at the middle of the ladder
			glm::vec3 characterNewPos{ minX.x + (maxX.x - minX.x) / 2.f, point.y,0 };
			characterNewPos.x -= frameWidthDivBy2;
			pTransform->SetLocalPosition(characterNewPos);
		}

		pCharacterComp->SetMovementUpDown(canMove);
	}
}

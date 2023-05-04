#include "LadderComponent.h"

#include "AnimationComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "RigidBodyComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Shape.h"
#include <algorithm>

using namespace dae;
LadderComponent::LadderComponent(GameObject* pOwner, RectangleShape* pShape) : Component(pOwner)
{
	m_pRigidBodyComp = pOwner->AddComponent<RigidBodyComponent>(pOwner->GetComponent<Transform>());
	MG_ASSERT(m_pRigidBodyComp != nullptr)
	m_pRigidBodyComp->SetShape(pShape);

}


void LadderComponent::Update(float)
{
	auto pScene = SceneManager::GetInstance().GetScene();
	MG_ASSERT(pScene != nullptr);
	auto characters = pScene->GetSceneCharacters();
	for(auto character: characters )
	{
		auto pTransform = character->GetComponent<Transform>();
		MG_ASSERT(pTransform != nullptr);
		auto pCharacterComp = character->GetComponent<CharacterComponent>();
		MG_ASSERT(pCharacterComp != nullptr);
		auto pRenderComp = character->GetComponent<RenderComponent>();
		MG_ASSERT(pRenderComp != nullptr);

		const float pFrameHeight = pRenderComp->GetFrameHeightScaled();
		auto point = pTransform->GetLocalPosition();
		auto pointVec2 = glm::ivec2{ point.x,point.y + pFrameHeight };
		bool canMove = m_pRigidBodyComp->IsPointInRect(pointVec2);
		auto points = m_pRigidBodyComp->GetShape()->GetPoints();
		glm::ivec2 minY = *std::min_element(points.begin(), points.end(), [](glm::ivec2& point1, glm::ivec2& point2)
			{
				return point1.y < point2.y;
			});

		auto maxY = *std::max_element(points.begin(), points.end(), [](glm::ivec2& point1, glm::ivec2& point2)
			{
				return point1.y < point2.y;
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
		pCharacterComp->SetMovementUpDown(canMove);
	}
}

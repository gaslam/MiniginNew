#include "PlatformComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "RigidBodyComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Shape.h"
#include <algorithm>

using namespace dae;
PlatformComponent::PlatformComponent(GameObject* pOwner, RectangleShape* pShape) : Component(pOwner)
{
	m_pRigidBodyComp = pOwner->AddComponent<RigidBodyComponent>(pOwner->GetComponent<Transform>());
	MG_ASSERT(m_pRigidBodyComp != nullptr)
		m_pRigidBodyComp->SetShape(pShape);

}


void PlatformComponent::Update(float)
{
	const auto pScene = SceneManager::GetInstance().GetScene();
	MG_ASSERT(pScene != nullptr);
	const auto characters = pScene->GetSceneCharacters();
	for (const auto character : characters)
	{
		const auto pTransform = character->GetComponent<Transform>();
		MG_ASSERT(pTransform != nullptr);
		const auto pCharacterComp = character->GetComponent<CharacterComponent>();
		MG_ASSERT(pCharacterComp != nullptr);
		const auto pRenderComp = character->GetComponent<RenderComponent>();
		MG_ASSERT(pRenderComp != nullptr);

		const int frameWidth = static_cast<int>(pRenderComp->GetFrameWidthScaled());
		const int frameHeight =static_cast<int>( pRenderComp->GetFrameHeightScaled());
		auto point = pTransform->GetLocalPosition();
		auto pointVec2 = glm::ivec2{ point.x + frameWidth,point.y + frameHeight };
		bool canMove = m_pRigidBodyComp->IsPointInRect(pointVec2);
		auto points = m_pRigidBodyComp->GetShape()->GetPoints();
		glm::ivec2 minX = *std::min_element(points.begin(),points.end(), [](const glm::ivec2& point1, const glm::ivec2& point2)
		{
			return point1.x < point2.x;
		});

		auto maxX = *std::max_element(points.begin(),points.end(), [](const glm::ivec2& point1, const glm::ivec2& point2)
		{
			return point1.x < point2.x;
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
		pCharacterComp->SetMovementLeftRight(canMove);
	}
}
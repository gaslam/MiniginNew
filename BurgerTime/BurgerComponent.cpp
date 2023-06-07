#include "BurgerComponent.h"

#include "CharacterManager.h"
#include "Shape.h"
#include "Components/RenderComponent.h"
#include "Components/RigidBodyComponent.h"

using namespace dae;

BurgerComponent::BurgerComponent(GameObject* pObject, const std::string& file) : Component{ pObject }
{
	auto pOwner{ GetOwner() };
	RenderComponent* pRenderComponent{ pOwner->GetComponent<RenderComponent>() };
	if(!pRenderComponent)
	{
		pRenderComponent = pOwner->AddComponent<RenderComponent>(file);
	}
	pRenderComponent->SetScale(2.5f);
	Transform* pTransform{ pOwner->GetComponent<Transform>() };
	if (!pTransform)
	{
		pTransform = pOwner->AddComponent<Transform>();
	}
	const int width{ static_cast<int>(pRenderComponent->GetFrameWidthScaled()) };
	const int height{ static_cast<int>(pRenderComponent->GetFrameHeightScaled()) };
	const glm::ivec2 pos{pTransform->GetLocalPosition()};
	const auto pShape{ new RectangleShape{ pos, width,height } };
	const auto pRigidBody{ pOwner->AddComponent<RigidBodyComponent>(pTransform) };
	pRigidBody->SetShape(pShape);
}

void BurgerComponent::Update(float /*elapsedTime*/)
{
	//CharacterManager::GetInstance().GetCharacters();
}

#include "SceneComponent.h"

#include "../Components/AudioComponent.h"
#include <Misc/GameObject.h>

#include "CharacterComponent.h"
#include "../Managers/CharacterManager.h"

using namespace dae;

SceneComponent::SceneComponent(GameObject* gameObject, glm::vec2 playerPos,float worldScale) : Component(gameObject), m_PlayerPos{ playerPos * worldScale }
{
	auto pOwner{ GetOwner() };
	if (!m_pBackgroundSound)
	{
		m_pBackgroundSound = pOwner->AddComponent<AudioComponent>();
	}

	m_pBackgroundSound->Load("Sound/background.wav");
	m_pBackgroundSound->SetLoops(-1);
}

void SceneComponent::Start()
{
	m_pBackgroundSound->Play();
	auto pPlayer{ CharacterManager::GetInstance().GetPlayer() };
	const auto characterComp{ pPlayer->GetComponent<CharacterComponent>() };
	characterComp->Reset(m_PlayerPos);
}

void SceneComponent::End()
{
	if(m_pBackgroundSound)
	{
		m_pBackgroundSound->StopAll();
	}

}

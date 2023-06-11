#include "SceneComponent.h"

#include "../Components/AudioComponent.h"
#include <Misc/GameObject.h>

using namespace dae;

SceneComponent::SceneComponent(GameObject* gameObject) : Component(gameObject)
{
	auto pOwner{ GetOwner() };
	if (!m_pBackgroundSound)
	{
		m_pBackgroundSound = pOwner->AddComponent<AudioComponent>();
	}

	m_pBackgroundSound->Load("Sound/music.wav");
	m_pBackgroundSound->Load("Sound/background.wav");
	m_pBackgroundSound->SetLoops(-1);
}

void SceneComponent::Start()
{
	m_pBackgroundSound->Play();
}

void SceneComponent::End()
{
	if(m_pBackgroundSound)
	{
		m_pBackgroundSound->StopAll();
	}

}

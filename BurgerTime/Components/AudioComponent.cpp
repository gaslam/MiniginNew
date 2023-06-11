#include "AudioComponent.h"
#include "imgui.h"
#include "Utils/Utils.h"

using namespace dae;

unsigned int AudioComponent::Load(const std::string& filePath)
{
	 m_Id = m_AudioBase->Load(filePath);
	 return m_Id;
}

void AudioComponent::Play() const
{
	if(!m_AudioBase->IsPlaying(m_Id))
	{
		m_AudioBase->Play(m_Id, m_Loops);
	}
}

void AudioComponent::Pause() const
{
	if(m_AudioBase->IsPlaying(m_Id))
	{
		m_AudioBase->Pause(m_Id);
	}
}

void AudioComponent::PauseAll() const
{
	m_AudioBase->PauseAll();
}

void AudioComponent::UnPause() const
{
	if (!m_AudioBase->IsPlaying(m_Id))
	{
		m_AudioBase->UnPause(m_Id);
	}
}

void AudioComponent::UnPauseAll() const
{
	m_AudioBase->UnPauseAll();
}

void AudioComponent::StopAll() const
{
	m_AudioBase->StopAll();
}

void AudioComponent::Stop() const
{
	m_AudioBase->Stop(m_Id);
}

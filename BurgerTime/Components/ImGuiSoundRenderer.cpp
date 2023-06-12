#include "ImGuiSoundRenderer.h"

#include "imgui.h"
#include <Audio/AudioBase.h>
#include "Misc/GameObject.h"
#include <Misc/Locator.h>
#include "Utils/Utils.h"

using namespace dae;

ImGuiSoundRenderer::ImGuiSoundRenderer(GameObject* pObject) : Component(pObject), m_pAudioBase{ Locator::GetAudio() }
{
	m_Volume = m_pAudioBase->GetVolume();
}

void ImGuiSoundRenderer::RenderImGUI()
{
	if (ImGui::CollapsingHeader("Sound"))
	{
		const float oldVolume = m_Volume;
		float min = 0.f;
		float max = 1.f;
		ImGui::SliderFloat("Volume ", &m_Volume, min, max, "%.1f");

		if (!utils::CompareFloats(oldVolume, m_Volume) && !m_IsMuted)
		{
			m_pAudioBase->SetVolume(m_Volume);
		}

		if (ImGui::Checkbox("Mute", &m_IsMuted))
		{
			const float volume{ m_IsMuted ? 0 : m_Volume };
			m_pAudioBase->SetVolume(volume);
		}
	}
}

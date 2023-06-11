#pragma once
#include <Misc\Component.h>

class AudioBase;
namespace dae
{

	class ImGuiSoundRenderer :
		public Component
	{
	public:
		ImGuiSoundRenderer(GameObject* pObject);
		void RenderImGUI() override;
	private:
		bool m_IsMuted{false};
		float m_Volume{};
		AudioBase* m_pAudioBase{};
	};
}


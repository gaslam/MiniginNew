#pragma once
#include <Misc/Component.h>

#include <Misc/Locator.h>
class AudioBase;

namespace dae
{
	class GameObject;
	class AudioComponent :
		public Component
	{
	public:
		AudioComponent(GameObject* object) : Component(object), m_AudioBase{Locator::GetAudio()}
		{
		}

		unsigned int Load(const std::string& filePath);
		void Play() const;
		void Pause() const;
		void PauseAll() const;
		void UnPause() const;
		void UnPauseAll() const;
		void StopAll() const;
		void Stop() const;
		void SetLoops(int loops) { m_Loops = loops; }
		void RenderImGUI() override;

	private:
		unsigned int m_Id{};
		int m_Loops{ 0 };
		AudioBase* m_AudioBase{};
	};
}



#include "Locator.h"

std::unique_ptr<AudioBase> Locator::m_pAudio{};

AudioBase* Locator::GetAudio()
{
	return  m_pAudio.get();
}

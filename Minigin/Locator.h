#pragma once
#include <memory>
#include "AudioBase.h"
#include "Logger.h"

class AudioDefault : public AudioBase
{
public:
    void Init() override {};
    void Play(unsigned int,int) override{};
    void Pause(unsigned int) override {}
    void PauseAll() override {}
    void UnPause(unsigned int) override {}
    void UnPauseAll() override{}
    void Stop(unsigned int) override {}
    void StopAll() override{};
	int Load(const std::string&) override { return -1; }
    bool IsPlaying(int) override { return false; }
    void OnSoundEnd(int) override{};
	void SetVolume(float) override {};
    float GetVolume() const override { return 0.5f; }
};

class Locator final
{
public:
    static AudioBase* GetAudio();

    static void Provide(std::unique_ptr<AudioBase>&& ss)
    {
        m_pAudio = ss == nullptr ? std::make_unique<AudioDefault>() : std::move(ss);
        MG_ASSERT(m_pAudio != nullptr, "Cannot get audio!!");
        m_pAudio->Init();

    }

private:
    static std::unique_ptr<AudioBase> m_pAudio;
};

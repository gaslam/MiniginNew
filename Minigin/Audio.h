#pragma once
#define WIN32_LEAN_AND_MEAN
#include "AudioBase.h"
#include <mutex>

class Audio :
	public AudioBase
{
public:
	explicit Audio();
	~Audio() override;

	Audio(const Audio& other) = delete;
	Audio(const Audio&& other) noexcept = delete;
	Audio&& operator=(const Audio& other) = delete;
	Audio&& operator=(Audio&& other) noexcept = delete;

	void Init() override;
	void Play(unsigned int soundID, int loops) override;
	void Pause(unsigned int soundID) override;
	void PauseAll() override;
	void UnPause(unsigned int soundID) override;
	void UnPauseAll() override;
	void Stop(unsigned int soundID) override;
	void StopAll() override;
	void OnSoundEnd(int channel) override;
	int Load(const std::string& filePath) override;
	bool IsPlaying(int channel) override;
	void SetVolume(float volume) override;
	float GetVolume() const override;
private:
	class AudioImpl;
	AudioImpl* m_Impl;
};


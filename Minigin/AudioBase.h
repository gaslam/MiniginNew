#pragma once
#include <string>

struct Mix_Chunk;
struct AudioInfo
{
	float volume{};
	std::string filePath{};
	int loops{ -1 };
	int channel{ -1 };
	Mix_Chunk* pData{};
};

class AudioBase
{
public:
	explicit AudioBase() = default;
	virtual ~AudioBase() = default;

	AudioBase(const AudioBase& other) = delete;
	AudioBase(AudioBase&& other) noexcept = delete;
	AudioBase& operator=(const AudioBase& other) = delete;
	AudioBase& operator=(AudioBase&& other) noexcept = delete;

	virtual void Init() = 0;
	virtual void Play(unsigned int soundID, int loops) = 0;
	virtual bool IsPlaying(int channel) = 0;
	virtual void Pause(unsigned int soundID) = 0;
	virtual void PauseAll() = 0;
	virtual void UnPause(unsigned int soundID) = 0;
	virtual void UnPauseAll() = 0;
	virtual void StopAll() = 0;
	virtual void Stop(unsigned int soundID) = 0;
	virtual int Load(const std::string& filePath) = 0;
	virtual void OnSoundEnd(int channel) = 0;
	virtual void SetVolume(float) = 0;
	virtual float GetVolume() const = 0;
};

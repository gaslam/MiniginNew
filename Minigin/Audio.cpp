#define WIN32_LEAN_AND_MEAN
#include "Audio.h"

#include <SDL.h>
#include <SDL_mixer.h>

#include "Locator.h"
#include "Logger.h"
#include <algorithm>

#include <thread>
#include <vector>
#include "CyclicBuffer.h"

class Audio::AudioImpl  final {
public:
	explicit AudioImpl() = default;
	~AudioImpl();

	AudioImpl(const AudioImpl& other) = delete;
	AudioImpl(const AudioImpl&& other) noexcept = delete;
	AudioImpl&& operator=(const AudioImpl& other) = delete;
	AudioImpl&& operator=(AudioImpl&& other) noexcept = delete;
	void Init();
	void Play(unsigned int soundID, int loops);
	void Pause(unsigned int soundID);
	void PauseAll();
	void UnPause(unsigned int soundID);
	void UnPauseAll();
	void Stop(unsigned int soundID);
	void StopAll();
	int Load(const std::string& filePath);
	void OnSoundEnd(int channel);
	bool IsPlaying(int channel);
	void SetVolume(float volume);
	float GetVolume() const { return m_Volume; }
private:
	enum class AudioEventType : int {
		PLAY,
		PAUSE,
		PAUSEALL,
		UNPAUSE,
		UNPAUSEALL,
		STOP,
		STOPALL,
		SETVOLUME
	};
	struct AudioEvent
	{
		AudioInfo info{};
		AudioEventType type{};
	};
	void PollEvents();
	static void EndSound(int channel);
	AudioEvent GenerateEvent(AudioEventType type, int channel, float volume, Mix_Chunk* pData, int loops) const;

	constexpr static int bufferSize{ 16 };
	CyclicBuffer<AudioEvent, bufferSize> m_AudioEventBuffer{};
	std::vector<AudioInfo> m_Sounds{};

	std::jthread m_AudioThread;
	std::mutex m_Mutex;
	float m_Volume{1.f};
};

Audio::Audio() : m_Impl(new AudioImpl{})
{
}

Audio::~Audio()
{
	delete m_Impl;
}

void Audio::Init()
{
	m_Impl->Init();
}

void Audio::Play(unsigned int soundID, int loops)
{
	m_Impl->Play(soundID, loops);
}

void Audio::Pause(unsigned int soundID)
{
	m_Impl->Pause(soundID);
}

void Audio::PauseAll()
{
	m_Impl->PauseAll();
}

void Audio::UnPause(unsigned int soundID)
{
	m_Impl->UnPause(soundID);
}

void Audio::UnPauseAll()
{
	m_Impl->UnPauseAll();
}

void Audio::Stop(unsigned int soundID)
{
	m_Impl->Stop(soundID);
}

void Audio::StopAll()
{
	m_Impl->StopAll();
}

void Audio::OnSoundEnd(int channel)
{
	m_Impl->OnSoundEnd(channel);
}

int Audio::Load(const std::string& filePath)
{
	return m_Impl->Load(filePath);
}

void Audio::AudioImpl::OnSoundEnd(int channel)
{
	std::lock_guard lock(m_Mutex);
	const auto it{ std::ranges::find_if(m_Sounds,[channel](const AudioInfo& info)
	{
		return info.channel == channel;
	}) };
	it->channel = -1;
}

bool Audio::AudioImpl::IsPlaying(int channel)
{
	return Mix_Playing(channel) == 1;
}

void Audio::AudioImpl::SetVolume(float volume)
{
	const AudioEvent event{ GenerateEvent(AudioEventType::SETVOLUME, -1, 0, nullptr, 0) };
	for(auto& sound: m_Sounds)
	{
		sound.volume = volume;
	}

	m_AudioEventBuffer.write(event);
	m_Volume = volume;
}

void Audio::AudioImpl::PollEvents()
{
	const std::stop_token& token { m_AudioThread.get_stop_token()};
	while (!token.stop_requested())
	{
		AudioEvent e{};

		{
			const std::lock_guard lock(m_Mutex);
			if (m_AudioEventBuffer.Size() <= 0)
			{
				continue;
			}

			e = m_AudioEventBuffer.pop();
		}

			switch (e.type)
			{
			case AudioEventType::PLAY:
			{
				if (IsPlaying(e.info.channel))
				{
					break;
				}
				const int channel{ Mix_PlayChannel(e.info.channel, e.info.pData,e.info.loops) };
				if (channel == -1)
				{
					MG_ASSERT_WARNING(channel == -1, "Cannot play audio file!!")
				}
				Mix_MasterVolume(static_cast<int>(m_Volume * MIX_MAX_VOLUME));
				break;
			}
			case AudioEventType::STOP:
			{
				const int channel{ Mix_HaltChannel(e.info.channel) };
				if (channel == -1)
				{
					MG_ASSERT_WARNING(channel == -1, "Cannot play audio file!!")
				}
				break;
			}
			case AudioEventType::STOPALL:
			{
				Mix_HaltChannel(-1);
				break;
			}
			case AudioEventType::PAUSE:
			{
				if (!IsPlaying(e.info.channel))
				{
					break;
				}
				Mix_Pause(e.info.channel);
				break;
			}
			case AudioEventType::UNPAUSE:
			{
				if (IsPlaying(e.info.channel))
				{
					break;
				}
				Mix_Resume(e.info.channel);
				break;
			}
			case AudioEventType::PAUSEALL:
			{
				Mix_HaltChannel(-1);
				break;
			}
			case AudioEventType::UNPAUSEALL:
				//Not working for some reason
				Mix_Resume(-1);
				Mix_ResumeMusic();
				break;
			case AudioEventType::SETVOLUME:
			{
				Mix_MasterVolume(static_cast<int>(m_Volume * MIX_MAX_VOLUME));
			}
			default:
				break;
			}
		}
}

bool Audio::IsPlaying(int channel)
{
	return m_Impl->IsPlaying(channel);
}

void Audio::SetVolume(float volume)
{
	m_Impl->SetVolume(volume);
}

float Audio::GetVolume() const
{
	return m_Impl->GetVolume();
}

void Audio::AudioImpl::EndSound(int channel)
{
	Locator::GetAudio()->OnSoundEnd(channel);
}

Audio::AudioImpl::AudioEvent Audio::AudioImpl::GenerateEvent(AudioEventType type, int channel, float volume, Mix_Chunk* pData, int loops) const
{
	AudioEvent event{};
	event.info.channel = channel;
	event.info.pData = pData;
	event.info.volume = volume;
	event.type = type;
	event.info.loops = loops;
	return event;
}

Audio::AudioImpl::~AudioImpl()
{
	m_AudioThread.request_stop();

	for (const auto sound : m_Sounds)
	{
		Mix_FreeChunk(sound.pData);
	}
}

void Audio::AudioImpl::Init()
{
	int result{ SDL_Init(SDL_INIT_AUDIO) };
	if (result < 0)
	{
		std::stringstream ss;
		ss << "Cannot Initialise SDL Audio. Error(s): " << SDL_GetError();
		MG_ASSERT(result >= 0, ss.str().c_str())
	}

	constexpr  int freq{ 44100 };
	constexpr Uint16 format { AUDIO_S16SYS};
	constexpr int channels { 2};
	constexpr int samples { 4096};

	result = Mix_OpenAudio(freq, format, channels, samples);

	if (result < 0)
	{
		std::stringstream ss;
		ss << "Cannot Initialise SDL Mixer. Error(s): " << Mix_GetError();
		MG_ASSERT(result != 0, ss.str().c_str())
	}

	m_AudioThread = std::jthread([this]() {PollEvents(); });

	Mix_ChannelFinished(&EndSound);
}

void Audio::AudioImpl::Play(unsigned int soundID, int loops)
{
	const std::lock_guard lock{m_Mutex};
	m_Sounds[soundID].loops = loops;
	auto sound{ m_Sounds[soundID] };
	sound.loops = loops;
	const AudioEvent event{ GenerateEvent(AudioEventType::PLAY, sound.channel, sound.volume, sound.pData, sound.loops) };
	m_AudioEventBuffer.write(event);
}

void Audio::AudioImpl::Pause(unsigned int soundID)
{
	const std::lock_guard lock{m_Mutex};
	const auto sound{ m_Sounds[soundID] };
	const AudioEvent event{ GenerateEvent(AudioEventType::PAUSE, sound.channel, sound.volume, sound.pData, sound.loops) };
	m_AudioEventBuffer.write(event);
}

void Audio::AudioImpl::PauseAll()
{
	const AudioEvent event { GenerateEvent(AudioEventType::PAUSEALL, 0, 0, nullptr, 0)};
	const std::lock_guard lock{m_Mutex};
	m_AudioEventBuffer.write(event);
}

void Audio::AudioImpl::UnPause(unsigned int soundID)
{
	const std::lock_guard lock{m_Mutex};
	const auto sound { m_Sounds[soundID]};
	const AudioEvent event { GenerateEvent(AudioEventType::UNPAUSE, sound.channel, sound.volume, sound.pData, sound.loops)};
	m_AudioEventBuffer.write(event);
}

void Audio::AudioImpl::UnPauseAll()
{
	const std::lock_guard lock{m_Mutex};
	const AudioEvent event { GenerateEvent(AudioEventType::UNPAUSEALL, 0, 0, nullptr, 0)};
	m_AudioEventBuffer.write(event);
}

void Audio::AudioImpl::Stop(unsigned int soundID)
{
	const std::lock_guard lock{m_Mutex};
	const auto sound { m_Sounds[soundID]};
	const AudioEvent event { GenerateEvent(AudioEventType::STOP, soundID, sound.volume, sound.pData, sound.loops)};
	m_AudioEventBuffer.write(event);
}

void Audio::AudioImpl::StopAll()
{
	const AudioEvent event { GenerateEvent(AudioEventType::STOPALL, 0, 0, nullptr, 0)};
	const std::lock_guard lock{m_Mutex};
	m_AudioEventBuffer.write(event);
}

int Audio::AudioImpl::Load(const std::string& filePath)
{
	const std::string fullDir { "../Data/" + filePath};

	const auto sound{ std::find_if(m_Sounds.begin(),m_Sounds.end(),[filePath](const AudioInfo& info)
	{
		return info.filePath == filePath;
	}) };

	Mix_Chunk* audioData{ };

	if(sound != m_Sounds.end())
	{
		audioData = sound->pData;
	}
	else
	{
		audioData = Mix_LoadWAV(fullDir.c_str());
	}

	const bool isDataLoaded{ audioData != nullptr };
	MG_ASSERT(audioData != nullptr, "Cannot load file!!")
		if (!isDataLoaded)
		{
			return -1;
		}

	std::lock_guard<std::mutex> lock(m_Mutex);
	AudioInfo audioInfo{};
	audioInfo.pData = audioData;
	audioInfo.volume = 0.5f;
	audioInfo.channel = static_cast<int>(m_Sounds.size());
	audioInfo.filePath = filePath;

	m_Sounds.emplace_back(audioInfo);

	return audioInfo.channel;
}

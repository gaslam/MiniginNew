
#include "CharacterComponent.h"
#include "AnimationComponent.h"
#include <algorithm>
#include <imgui.h>

#include "GameObject.h"
#include "Audio.h"

dae::CharacterComponent::CharacterComponent(GameObject* pOwner, AnimationComponent* pComponent, AudioBase* pAudio) : Component(pOwner), m_pAnimationComponent{ pComponent }, m_pAudio{ pAudio }, m_CharacterState{}
{
	int channel = m_pAudio->Load("Sound/bounce.wav");
	const char* sound = "walk";
	int timesToPlay = -1;
	std::pair<int, int> channelAndTimesPlay{ channel,timesToPlay };
	m_pSoundChannels.emplace(sound, channelAndTimesPlay);
}

void dae::CharacterComponent::Update(float)
{
	m_CanSetMoveLeftRight = true;
	m_CanSetMoveUpDown = true;
}

void dae::CharacterComponent::RenderImGUI() const
{
	ImGui::Text("Player:");
	auto pTransform = GetOwner()->GetComponent<Transform>();
	const auto position = pTransform->GetLocalPosition();
	ImGui::NewLine();
	ImGui::Text("Position:");
	ImGui::Text("x = %.2f", position.x);
	ImGui::Text("y = %.2f", position.y);
	ImGui::NewLine();
	ImGui::Text("Sounds:");
	for (auto pair : m_pSoundChannels)
	{
		std::string buttonText{ "Play "};
		buttonText += pair.first;
		if (ImGui::Button(buttonText.c_str()))
		{
			m_pAudio->Play(pair.second.first, 0);
		}
	}

	for (auto pair : m_pSoundChannels)
	{
		std::string buttonText{ "Stop " };
		buttonText += pair.first;
		if (ImGui::Button(buttonText.c_str()))
		{
			m_pAudio->Stop(pair.second.first);
		}
	}

	for (auto pair : m_pSoundChannels)
	{
		std::string buttonText{ "Pause " };
		buttonText += pair.first;
		if (ImGui::Button(buttonText.c_str()))
		{
			m_pAudio->Pause(pair.second.first);
		}
	}

	for (auto pair : m_pSoundChannels)
	{
		std::string buttonText{ "Unpause " };
		buttonText += pair.first;
		if (ImGui::Button(buttonText.c_str()))
		{
			m_pAudio->UnPause(pair.second.first);
		}
	}

	if (ImGui::Button("Pause all"))
	{
		m_pAudio->PauseAll();
	}

	if (ImGui::Button("Unpause all"))
	{
		m_pAudio->UnPauseAll();
	}

	ImGui::NewLine();

	float volume = m_pAudio->GetVolume();
	const float oldVolume = volume;
	float min = 0.f;
	float max = 1.f;
	ImGui::SliderFloat("Volume ",&volume,min, max, "%.1f");

	if(oldVolume != volume)
	{
		m_pAudio->SetVolume(volume);
	}
}

void dae::CharacterComponent::AddAnimation(AnimationItem& animation, CharacterState& state)
{
	MG_ASSERT(animation.count > -1,"Animation count must be 0 or higher!!");
	MG_ASSERT(animation.startCol > -1, "The column where the animation starts must have an index of 0 or higher!!");
	MG_ASSERT(animation.startRow > -1, "The row where the animation starts must have an index of 0 or higher!!");
	m_Animations.emplace(state, animation);
}

void dae::CharacterComponent::SetAnimation(CharacterState& state)
{
	auto it = std::find_if(m_Animations.begin(), m_Animations.end(), [state](std::pair<CharacterState, dae::AnimationItem> pair) {
		return pair.first == state;
		});
	if (it == m_Animations.end())
	{
		return;
	}

	auto pair = *it;
	AnimationItem item = pair.second;
	if (m_pAnimationComponent)
	{
		m_CharacterState = state;
		auto sound = m_pSoundChannels["walk"];
		if (m_CharacterState == moveLeft || m_CharacterState == moveRight)
		{
			m_pAudio->Play(sound.first, sound.second);
		}
		else
		{
			m_pAudio->Stop(sound.first);
		}
		m_pAnimationComponent->ChangeAnimation(item.startRow,item.startCol,item.count,item.isRepeatable,item.isXflipped,item.isYflipped);
	}
}

void dae::CharacterComponent::SetAnimation(int id)
{
	CharacterState state{ id };
	SetAnimation(state);
}

void dae::CharacterComponent::SetState(CharacterState& state)
{
	if (m_CharacterState == state)
	{
		return;
	}
	SetAnimation(state);
}

void dae::CharacterComponent::SetMovementLeftRight(bool canMove)
{
	if(m_CanSetMoveLeftRight)
	{
		m_CanMoveLeftRight = canMove;
	}
	if(canMove)
	{
		m_CanSetMoveLeftRight = false;
	}
}

void dae::CharacterComponent::SetMovementUpDown(bool canMove)
{
	if(m_CanSetMoveUpDown)
	{
		m_CanMoveUpDown = canMove;
	}
	if(canMove)
	{
		m_CanSetMoveUpDown = false;
	}
}

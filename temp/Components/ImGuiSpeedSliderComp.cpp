#include "ImGuiSpeedSliderComp.h"

void dae::ImGuiSpeedSliderComp::SetMinMaxSpeed(float min, float max)
{
	m_MinSpeed = min;
	m_MaxSpeed = max;
}

void dae::ImGuiSpeedSliderComp::Render() const
{
	auto pOwner = GetOwner();
	if (!pOwner)
	{
		return;
	}
	ImGui::Begin(m_Title.c_str(), nullptr, ImGuiWindowFlags_None);
	ImGui::Text("Speed variables:");
	//works but does weird stuff. I hope I did do anything too bad in the background
	for (size_t i{}; i < m_TransformComponents.size(); ++i)
	{
		auto pTransform = m_TransformComponents[i];
		if (!pTransform)
		{
			continue;
		}
		float speed{ pTransform->GetSpeedForMovement() };
		float prevSpeed{ pTransform->GetSpeedForMovement() };
		ImGui::SliderFloat("Speed", &speed, m_MinSpeed, m_MaxSpeed);
		if (prevSpeed != speed)
		{
			prevSpeed += 1.f;
		}
		pTransform->SetSpeedForMovement(speed);
	}

	ImGui::Text("Controls:");
	ImGui::NewLine();
	ImGui::Text("Keyboard:");
	ImGui::Text("Movement: WASD");
	ImGui::NewLine();
	ImGui::Text("Controller:");
	ImGui::Text("Movement: Left-joystick or DPAD");
	ImGui::End();
}

void dae::ImGuiSpeedSliderComp::AddTransform(Transform* transform)
{
	m_TransformComponents.emplace_back(transform);
}

#include "XboxController.h"

using namespace dae;

XboxController::XboxController(unsigned int controllerIdx) :
	m_ControllerIndex{controllerIdx}
{

}

void XboxController::Update()
{
	CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(m_ControllerIndex, &m_CurrentState);

	auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonsPressedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
}

bool XboxController::IsButtonDown(unsigned int button) const
{
	return m_ButtonsPressedThisFrame & button;
}

bool XboxController::IsUpThisFrame(unsigned int button) const
{
	return m_ButtonsReleasedFrame & button;
}

bool XboxController::IsPressed(unsigned int button) const
{
	return m_CurrentState.Gamepad.wButtons & button;
}

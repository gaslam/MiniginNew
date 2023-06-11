#define WIN32_LEAN_AND_MEAN
#include "XboxController.h"

#include <windows.h>
#include <Xinput.h>

using namespace dae;

class XboxController::XboxControllerImpl final
{
	XINPUT_STATE m_CurrentState{};
	XINPUT_STATE m_PreviousState{};
	int m_ButtonsPressedThisFrame{};
	int m_ButtonsReleasedFrame{};

	unsigned int m_ControllerIndex{};
public:
	explicit XboxControllerImpl(unsigned int controllerIndex) : m_ControllerIndex{controllerIndex}
	{
		
	}
	void Update()
	{
		CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
		XInputGetState(m_ControllerIndex, &m_CurrentState);

		auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
		m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
		m_ButtonsReleasedFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
	}
	bool IsButtonDown(unsigned int button) const
	{
		return m_ButtonsPressedThisFrame & button;
	}
	bool IsUpThisFrame(unsigned int button) const
	{
		return m_ButtonsReleasedFrame & button;
	}
	bool IsPressed(unsigned int button) const
	{
		return m_CurrentState.Gamepad.wButtons & button;
	}
	unsigned int GetID() const { return m_ControllerIndex; }
};

XboxController::XboxController(unsigned int controllerIndex) : m_pImpl{ new XboxControllerImpl{controllerIndex} }
{
}

XboxController::~XboxController()
{
	delete m_pImpl;
}

void XboxController::Update()
{
	m_pImpl->Update();
}

bool XboxController::IsButtonDown(unsigned int button) const
{
	return m_pImpl->IsButtonDown(button);
}

bool XboxController::IsUpThisFrame(unsigned int button) const
{
	return m_pImpl->IsUpThisFrame(button);
}

bool XboxController::IsPressed(unsigned int button) const
{
	return m_pImpl->IsPressed(button);
}

unsigned int XboxController::GetID() const
{
	return m_pImpl->GetID();
}

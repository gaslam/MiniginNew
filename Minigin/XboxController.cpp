#define WIN32_LEAN_AND_MEAN
#include "XboxController.h"

using namespace dae;

class XboxController::ControllerImpl final {
public:
	ControllerImpl(unsigned int controllerIdx);
	~ControllerImpl() = default;
	ControllerImpl(const ControllerImpl& other) = delete;
	ControllerImpl(ControllerImpl&& other) noexcept = delete;
	ControllerImpl& operator=(const ControllerImpl& other) = delete;
	ControllerImpl& operator=(ControllerImpl&& other) noexcept = delete;
	void Update();
	bool IsButtonDown(unsigned int button) const;
	bool IsUpThisFrame(unsigned int button) const;
	bool IsPressed(unsigned int button) const;
	unsigned int GetID() const;
private:
	XINPUT_STATE m_CurrentState{};
	XINPUT_STATE m_PreviousState{};
	int m_ButtonsPressedThisFrame{};
	int m_ButtonsReleasedFrame{};

	unsigned int m_ControllerIndex{};
};

XboxController::XboxController(unsigned int controllerIdx) :
	m_pImpl{ new ControllerImpl{controllerIdx} }
{

}

dae::XboxController::~XboxController()
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

unsigned int dae::XboxController::GetID() const
{
	return m_pImpl->GetID();
}

void XboxController::ControllerImpl::Update()
{
	CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(m_ControllerIndex, &m_CurrentState);

	auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonsReleasedFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
}

bool XboxController::ControllerImpl::IsButtonDown(unsigned int button) const
{
	return m_CurrentState.Gamepad.wButtons & button;
}

bool XboxController::ControllerImpl::IsUpThisFrame(unsigned int button) const
{
	return m_ButtonsReleasedFrame & button;
}

bool XboxController::ControllerImpl::IsPressed(unsigned int button) const
{
	return m_ButtonsPressedThisFrame & button;
}

unsigned int XboxController::ControllerImpl::GetID() const
{
	return m_ControllerIndex;
}

XboxController::ControllerImpl::ControllerImpl(unsigned int controllerIdx) : m_ControllerIndex{controllerIdx}
{
}

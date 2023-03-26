#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>
namespace dae {
	class XboxController final
	{
		XINPUT_STATE m_CurrentState{};
		XINPUT_STATE m_PreviousState{};
		int m_ButtonsPressedThisFrame{};
		int m_ButtonsReleasedFrame{};

		unsigned int m_ControllerIndex{};
	public:
		XboxController(unsigned int controllerIndex);
		void Update();
		bool IsButtonDown(unsigned int button) const;
		bool IsUpThisFrame(unsigned int button) const;
		bool IsPressed(unsigned int button) const;
		unsigned int GetID() const { return m_ControllerIndex; }

		enum class ControllerButton {
			DPadUp = 0x0001,
			DPadDown = 0x0002,
			DPadLeft = 0x0004,
			DPadRight = 0x0008
		};
	};
}


#pragma once
#include <windows.h>
#include <Xinput.h>
#include <memory>
namespace dae {
	class XboxController final
	{;
	public:
		XboxController(unsigned int controllerIndex);
		~XboxController();
		XboxController(const XboxController& other) = delete;
		XboxController& operator=(const XboxController& other) = delete;
		XboxController(XboxController&& other) noexcept = delete;
		XboxController& operator=(XboxController&& other) noexcept = delete;
		void Update();
		bool IsButtonDown(unsigned int button) const;
		bool IsUpThisFrame(unsigned int button) const;
		bool IsPressed(unsigned int button) const;
		unsigned int GetID() const;

		enum class ControllerButton {
			A = 0x1000,
			B = 0x2000,
			X = 0x4000,
			Y = 0x8000,
			LeftBumper = 0x0100,
			RightBumper = 0x0200,
			Back = 0x0020,
			Start = 0x0010,
			LeftStick = 0x0040,
			RightStick = 0x0080,
			DPadUp = 0x0001,
			DPadDown = 0x0002,
			DPadLeft = 0x0004,
			DPadRight = 0x0008

		};
	private:
		class ControllerImpl;
		ControllerImpl* m_pImpl{};
	};
}


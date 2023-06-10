#pragma once
namespace dae {
	class XboxController final
	{
	public:
		explicit XboxController(unsigned int controllerIndex);
		~XboxController();

		XboxController(const XboxController& other) = delete;
		XboxController(XboxController&& other) noexcept = delete;

		XboxController operator=(const XboxController& other) = delete;
		XboxController operator=(XboxController&& other) noexcept = delete;
		void Update();
		bool IsButtonDown(unsigned int button) const;
		bool IsUpThisFrame(unsigned int button) const;
		bool IsPressed(unsigned int button) const;
		unsigned int GetID() const;

		enum class ControllerButton {
			DPadUp = 0x0001,
			DPadDown = 0x0002,
			DPadLeft = 0x0004,
			DPadRight = 0x0008,
			Start = 0x0010,
			Back = 0x0020,
			LeftShoulder = 0x0100,
			RightShoulder = 0x0200,
			A = 0x1000,
			B = 0x2000,
			X = 0x4000,
			Y = 0x8000
		};
	private:

		class XboxControllerImpl;
		XboxControllerImpl* m_pImpl;
	};
}


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
			DPadRight = 0x0008
		};

		class XboxControllerImpl;
		XboxControllerImpl* m_pImpl;
	};
}


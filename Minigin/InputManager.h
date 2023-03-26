#include "Singleton.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>
#include "XboxController.h"
#include <utility>
#include <map>
#include <memory>
#include <vector>
#include "Command.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
		using ControllerKey = std::pair<unsigned, XboxController::ControllerButton>;
		using JoystickKey = std::pair<unsigned, int>;
		using CommandsMap = std::map<ControllerKey, std::unique_ptr<Command>>;
		using JoystickMap = std::map<JoystickKey, std::unique_ptr<Command>>;
		std::vector<std::unique_ptr<XboxController>> m_Controllers{};
		CommandsMap m_Commands{};
		JoystickMap m_JoystickCommands{};
		XINPUT_STATE m_CurrentState{};
		void HandleJoystick(bool isLeft, Command* command,XINPUT_STATE state);
	public:
		bool ProcessInput();
		void AddController(unsigned int id);
		void BindControllerToCommand(unsigned int controllerId, XboxController::ControllerButton& button, Command* command);
		void BindJoystickToCommand(unsigned int controllerId, Command* command, bool isLeft = true);
		void UpdateControls();
		void ProcessControllerInput();
	};

}

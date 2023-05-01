#pragma once
#include <windows.h>
#include "Singleton.h"
#include <utility>
#include <map>
#include <memory>
#include <vector>
#include "Command.h"
#include "XboxController.h"
#include <SDL.h>
namespace dae
{
	enum class KeyState : int {
		up,
		down,
		pressed
	};
	struct KeyInput {
		int id{};
		unsigned int controllerId{};
		XboxController::ControllerButton controllerButton{};
		SDL_Scancode scancode{};
		KeyState state{};

		bool operator<(const KeyInput& rhs) const
		{
			return this->id < rhs.id;
		}
	};
	class Input final :
		public Singleton<Input>
	{
		using JoystickKey = std::pair<unsigned, int>;
		using CommandsMap = std::map<KeyInput, std::unique_ptr<Command>>;
		using JoystickMap = std::map<JoystickKey, std::unique_ptr<Command>>;
		std::vector<std::unique_ptr<XboxController>> m_Controllers{};
		CommandsMap m_Commands{};
		JoystickMap m_JoystickCommands{};
		XINPUT_STATE m_CurrentState{};
		const Uint8* m_pKeyboardState{};

	public:
		[[nodiscard]] const std::vector<std::unique_ptr<XboxController>>& GetControllers() const { return m_Controllers; }
		[[nodiscard]] const CommandsMap& GetCommands() const { return m_Commands; }
		[[nodiscard]] const JoystickMap& GetJoystickCommands() const { return m_JoystickCommands; }

		void AddController(unsigned int id);
		void HandleJoystick(bool isLeft, Command* command, XINPUT_STATE state, float deltaTime);
		void BindButtonsToCommand(unsigned int id, XboxController::ControllerButton& button, SDL_Scancode& keyboardButton, KeyState& state, Command* command);
		void BindJoystickToCommand(unsigned int controllerId, Command* command, bool isLeft = true);
		void UpdateControls();
	};
}


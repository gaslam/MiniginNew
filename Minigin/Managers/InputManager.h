#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <Xinput.h>

#include "../Misc/Singleton.h"
#include "../Command/Command.h"
#include "../Controls/Keyboard.h"
#include "../Controls/XboxController.h"
#include "../Controls/KeyStructs.h"
enum class KeyState;
namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
		using JoystickKey = std::pair<unsigned, int>;
		using CommandsMap = std::map<KeyInput, std::unique_ptr<Command>>;
		using InputMap = std::map<std::string,KeyInput>;
		using JoystickMap = std::map<JoystickKey, std::unique_ptr<Command>>;
		std::vector<std::unique_ptr<XboxController>> m_Controllers{};
		CommandsMap m_Commands{};
		JoystickMap m_JoystickCommands{};
		InputMap m_Inputs{};
		InputMap m_pInputKeys{};
		XINPUT_STATE m_CurrentState{};
		const Uint8* m_pKeyboardState{};
		std::unique_ptr<Keyboard> m_pKeyboard{};
	public:
		InputManager();

		bool IsPressed(const SDL_Scancode& keyboardKey) const;
		void ProcessKeyboardInputPressed(float deltaTime) const;
		bool GetInputKeyUp(const std::string& value) const;
		bool GetInputKeyPressed(const std::string& value) const;
		bool GetInputKeyDown(const std::string& value) const;
		void ProcessKeyboardInputUp(float deltaTime) const;
		void ProcessKeyboardInputDown(float deltaTime) const;
		bool ProcessInput(float deltaTime);
		void ProcessControllerInput(float deltaTime);

		[[nodiscard]] const std::vector<std::unique_ptr<XboxController>>& GetControllers() const { return m_Controllers; }
		[[nodiscard]] const CommandsMap& GetCommands() const { return m_Commands; }
		[[nodiscard]] const JoystickMap& GetJoystickCommands() const { return m_JoystickCommands; }

		void AddController(unsigned int id);
		void BindButtonsToCommand(unsigned int id, XboxController::ControllerButton& button, SDL_Scancode& keyboardButton, KeyState& state, Command* command);
		void BindJoystickToCommand(unsigned int controllerId, Command* command, bool isLeft = true);
		void UpdateControls() const;
		void BindButtonsToInput(std::string& key, unsigned id, XboxController::ControllerButton& button, SDL_Scancode& keyboardButton, KeyState& state);
	private:
		void ProcessKeyboardInput(float deltaTime) const;
		void HandleJoystick(bool isLeft, Command* command, XINPUT_STATE state, float deltaTime);
	};

}

#include <windows.h>
#include <SDL.h>
#include "InputManager.h"

#include <algorithm>
#include <backends/imgui_impl_sdl.h>
#include <Xinput.h>
#include "../Command/Command.h"
#include <iostream>


bool dae::InputManager::ProcessInput(float deltaTime)
{
	m_pKeyboard->UpdateOldKeys();
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if(e.type == SDL_KEYDOWN)
		{
			const bool enabled{ true };
			m_pKeyboard->SetKey(e.key.keysym.scancode, enabled);
		}
		if (e.type == SDL_KEYUP)
		{
			const bool enabled{ false };
			m_pKeyboard->SetKey(e.key.keysym.scancode, enabled);
		}

		ImGui_ImplSDL2_ProcessEvent(&e);
	}
	ProcessKeyboardInput(deltaTime);
	ProcessControllerInput(deltaTime);

	return true;
}

void dae::InputManager::ProcessKeyboardInput(float deltaTime) const
{
	ProcessKeyboardInputDown(deltaTime);
	ProcessKeyboardInputUp(deltaTime);
	ProcessKeyboardInputPressed(deltaTime);
}

void dae::InputManager::AddController(unsigned int id)
{
	m_Controllers.emplace_back(std::make_unique<XboxController>(id));
}

void dae::InputManager::BindButtonsToCommand(unsigned int id, XboxController::ControllerButton& button, SDL_Scancode& keyboardButton, KeyState& state, Command* command)
{
	KeyInput input{};
	input.id = static_cast<int>(m_Commands.size());
	input.controllerId = id;
	input.controllerButton = button;
	input.scancode = keyboardButton;
	input.state = state;
	m_Commands.insert({ input, std::unique_ptr<Command>(command) });
}

void dae::InputManager::BindButtonsToInput(std::string& key, unsigned int id, XboxController::ControllerButton& button, SDL_Scancode& keyboardButton, KeyState& state)
{
	KeyInput input{};
	input.id = static_cast<int>(m_Inputs.size());
	input.controllerId = id;
	input.controllerButton = button;
	input.scancode = keyboardButton;
	input.state = state;
	m_Inputs.insert({ key, input });
}

void dae::InputManager::BindJoystickToCommand(unsigned int controllerId, Command* command, bool isLeft)
{
	int joystickIdx = isLeft ? 0 : 1;
	JoystickKey key = JoystickKey(controllerId, joystickIdx);
	m_JoystickCommands.insert({ key, std::unique_ptr<Command>(command) });
}

void dae::InputManager::UpdateControls() const
{
	for (auto& controller : m_Controllers)
	{
		controller->Update();
	}
}


void dae::InputManager::HandleJoystick(bool isLeft, Command* command, XINPUT_STATE state, float deltaTime)
{
	//determine deadzone based on whether the left or right joystick is used
	const int deadzone = isLeft ? XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE : XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	glm::vec2 dir{};
	dir.x = isLeft ? state.Gamepad.sThumbLX : state.Gamepad.sThumbRX;
	dir.y = isLeft ? state.Gamepad.sThumbLY : state.Gamepad.sThumbRY;

	//determine how far the controller is pushed. Tried to use it without, but for unit vectors, I could not find a way without square roots
	const float magnitude = sqrtf(dir.x * dir.x + dir.y * dir.y);

	//determine the direction the controller is pushed by normalizing it
	dir.x = dir.x / magnitude;
	dir.y = dir.y / magnitude;
	dir.y = -dir.y;

	//check if the controller is inside a circular dead zone
	if (magnitude < deadzone)
	{
		dir = {};
	}
	command->Execute(dir, deltaTime);
}


dae::InputManager::InputManager() : m_pKeyboard(std::make_unique<Keyboard>())
{
	const int maxPlayers{2};
	for(unsigned int i{}; i < maxPlayers; ++i)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// Simply get the state of the controller from XInput.
		DWORD dwResult = XInputGetState(i, &state);
		if(dwResult == ERROR_SUCCESS)
		{
			auto controller = std::make_unique<XboxController>(i);
			m_Controllers.emplace_back(std::move(controller));
		}
	}
}

bool dae::InputManager::IsPressed(const SDL_Scancode& keyboardKey) const
{
	return m_pKeyboard->IsPressed(keyboardKey);
}

void dae::InputManager::ProcessKeyboardInputPressed(float deltaTime) const
{
	auto& commands = GetCommands();
	for (auto& command : commands)
	{
		const KeyInput keyInput = command.first;
		if (IsPressed(keyInput.scancode) && keyInput.state == KeyState::pressed)
		{
			command.second->Execute(deltaTime);
		}
	}
}

bool dae::InputManager::GetInputKeyUp(const std::string& value) const
{
	auto it = std::find_if(m_Inputs.begin(), m_Inputs.end(), [value](const std::pair<std::string,KeyInput>& pair)
		{
			if(pair.first == value)
			{
				return true;
			}
			return false;
		});

	if(it == m_Inputs.end())
	{
		return false;
	}
	auto input = *it;
	if(m_pKeyboard->IsUpThisFrame(input.second.scancode))
	{
		return true;
	}

	for(auto& controller: m_Controllers)
	{
		if(controller->IsUpThisFrame(static_cast<int>(input.second.controllerButton)))
		{
			return true;
		}
	}
	return false;
}


bool dae::InputManager::GetInputKeyPressed(const std::string& value) const
{
	auto it = std::find_if(m_Inputs.begin(), m_Inputs.end(), [value](const std::pair<std::string, KeyInput>& pair)
		{
			if (pair.first == value)
			{
				return true;
			}
			return false;
		});

	if (it == m_Inputs.end())
	{
		return false;
	}
	auto input = *it;
	if (m_pKeyboard->IsPressed(input.second.scancode))
	{
		return true;
	}

	for (auto& controller : m_Controllers)
	{
		if (controller->IsPressed(static_cast<int>(input.second.controllerButton)))
		{
			return true;
		}
	}
	return false;
}

bool dae::InputManager::GetInputKeyDown(const std::string& value) const
{
	auto it = std::find_if(m_Inputs.begin(), m_Inputs.end(), [value](const std::pair<std::string, KeyInput>& pair)
		{
			if (pair.first == value)
			{
				return true;
			}
			return false;
		});

	if (it == m_Inputs.end())
	{
		return false;
	}
	auto input = *it;
	if (m_pKeyboard->IsButtonDown(input.second.scancode))
	{
		return true;
	}

	for (auto& controller : m_Controllers)
	{
		if (controller->IsButtonDown(static_cast<int>(input.second.controllerButton)))
		{
			return true;
		}
	}
	return false;
}

void dae::InputManager::ProcessKeyboardInputUp(float deltaTime) const
{
	auto& commands = GetCommands();
	for (auto& command : commands)
	{
		const KeyInput keyInput = command.first;
		if (keyInput.state == KeyState::up && m_pKeyboard->IsUpThisFrame(keyInput.scancode))
		{
			command.second->Execute(deltaTime);
		}
	}
}

void dae::InputManager::ProcessKeyboardInputDown(float deltaTime) const
{
	auto& commands = GetCommands();
	for (auto& command : commands)
	{
		const KeyInput keyInput = command.first;
		if (keyInput.state == KeyState::down && m_pKeyboard->IsUpThisFrame(keyInput.scancode))
		{
			command.second->Execute(deltaTime);
		}
	}
}

void dae::InputManager::ProcessControllerInput(float deltaTime)
{
	auto& commands = GetCommands();
	auto& joystickCommands = GetJoystickCommands();
	auto& controllers = GetControllers();

	for (auto& controller : controllers)
	{
		XINPUT_STATE state;
		DWORD result = XInputGetState(controller->GetID(), &state);
		if (result != ERROR_SUCCESS)
		{
			continue;
		}
		for (auto& command : commands)
		{
			const auto controllerKey = command.first.controllerButton;
			const unsigned int controllerId = command.first.controllerId;
			const bool isPressed{ controller->IsPressed(static_cast<int>(controllerKey)) && command.first.state == KeyState::pressed };
			const bool isDown{ controller->IsButtonDown(static_cast<int>(controllerKey)) && command.first.state == KeyState::down };
			const bool isUp{ controller->IsUpThisFrame(static_cast<int>(controllerKey)) && command.first.state == KeyState::up};
			if (controller->GetID() == controllerId && (isPressed || isDown || isUp))
			{
				command.second->Execute(deltaTime);
			}

		}

		for (auto& command : joystickCommands)
		{
			bool isLeft = command.first.second == 0;
			Command* actualCommand = command.second.get();
			const unsigned int controllerId = command.first.first;
			if (controller->GetID() == controllerId)
			{
				HandleJoystick(isLeft, actualCommand, state, deltaTime);
			}
		}
	}
}
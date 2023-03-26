#include <SDL.h>
#include "InputManager.h"
#include <backends/imgui_impl_sdl.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>
#include "Command.h"
#include <iostream>
#include <algorithm>
#include "Command.h"

bool dae::InputManager::ProcessInput()
{
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(0, &m_CurrentState);

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {

		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {

		}
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	ProcessControllerInput();

	return true;
}

void dae::InputManager::AddController(unsigned int id)
{
	m_Controllers.emplace_back(std::make_unique<XboxController>(id));
}

void dae::InputManager::BindControllerToCommand(unsigned int id, XboxController::ControllerButton& button, Command* command)
{
	ControllerKey key = ControllerKey(id, button);
	m_Commands.insert({ key, std::unique_ptr<Command>(command) });
}

void dae::InputManager::BindJoystickToCommand(unsigned int controllerId, Command* command, bool isLeft)
{
	int joystickIdx = isLeft ? 0 : 1;
	JoystickKey key = JoystickKey(controllerId, joystickIdx);
	m_JoystickCommands.insert({ key, std::unique_ptr<Command>(command) });
}

void dae::InputManager::UpdateControls()
{
	for (auto& controller : m_Controllers)
	{
		controller->Update();
	}
}

void dae::InputManager::HandleJoystick(bool isLeft, Command* command, XINPUT_STATE state)
{
	int deadzone = isLeft ? XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE : XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	glm::vec3 dir{};
	dir.x = isLeft ? state.Gamepad.sThumbLX : state.Gamepad.sThumbRX ;
	dir.y = isLeft ? state.Gamepad.sThumbLY : state.Gamepad.sThumbRY;

	//determine how far the controller is pushed
	float magnitude = sqrtf(dir.x * dir.x + dir.y * dir.y);

	//determine the direction the controller is pushed by normalizing it
	dir.x = dir.x / magnitude;
	dir.y = dir.y / magnitude;
	dir.y = -dir.y;

	//check if the controller is inside a circular dead zone
	if (magnitude < deadzone)
	{
		dir = {};
	}
	command->Execute(dir.x, dir.y);
}

void dae::InputManager::ProcessControllerInput()
{
	for (auto& controller : m_Controllers)
	{
		for (auto& command : m_Commands)
		{
			const auto controllerKey = command.first.second;
			const unsigned int controllerId = command.first.first;
			if (controller->GetID() == controllerId && controller->IsPressed(static_cast<int>(controllerKey)))
			{
				command.second->Execute();
			}

		}

		XINPUT_STATE state;
		XInputGetState(controller->GetID(), &state);
		for (auto& command : m_JoystickCommands)
		{
			bool isLeft = command.first.second == 0;
			Command* actualCommand = command.second.get();
			const unsigned int controllerId = command.first.first;
			if (controller->GetID() == controllerId)
			{
				HandleJoystick(isLeft, actualCommand, state);
			}
		}
	}
}

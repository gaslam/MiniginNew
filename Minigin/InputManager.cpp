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
#include "Input.h"

bool dae::InputManager::ProcessInput(float deltaTime)
{
	m_pKeyboardState = SDL_GetKeyboardState(nullptr);
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		else if (e.type == SDL_KEYDOWN) {
			ProcessKeyboardInputDown(deltaTime, e.key.keysym.scancode);
		}
		else if (e.type == SDL_KEYUP)
		{
			ProcessKeyboardInputUp(deltaTime, e.key.keysym.scancode);
		}

		ProcessKeyboardInputPressed(deltaTime);
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	ProcessControllerInput(deltaTime);

	return true;
}


bool dae::InputManager::IsPressed(const SDL_Scancode& keyboardKey)
{
	return m_pKeyboardState[keyboardKey];
}

void dae::InputManager::ProcessKeyboardInputPressed(float deltaTime)
{
	auto& input = Input::GetInstance();
	auto& commands = input.GetCommands();
	for (auto& command : commands)
	{
		const KeyInput keyInput = command.first;
		if (IsPressed(keyInput.scancode) && keyInput.state == KeyState::pressed)
		{
			command.second->Execute(deltaTime);
		}
	}
}

void dae::InputManager::ProcessKeyboardInputUp(float deltaTime, SDL_Scancode& e)
{
	auto& input = Input::GetInstance();
	auto& commands = input.GetCommands();
	for (auto& command : commands)
	{
		const KeyInput keyInput = command.first;
		if (keyInput.state == KeyState::up && e == keyInput.scancode)
		{
			command.second->Execute(deltaTime);
		}
	}
}

void dae::InputManager::ProcessKeyboardInputDown(float deltaTime, SDL_Scancode& e)
{
	auto& input = Input::GetInstance();
	auto& commands = input.GetCommands();
	for (auto& command : commands)
	{
		const KeyInput keyInput = command.first;
		if (keyInput.state == KeyState::down && e == keyInput.scancode)
		{
			command.second->Execute(deltaTime);
		}
	}
}

void dae::InputManager::ProcessControllerInput(float deltaTime)
{
	auto& input = Input::GetInstance();
	auto& commands = input.GetCommands();
	auto& joystickCommands = input.GetJoystickCommands();
	auto& controllers = input.GetControllers();

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
			if (controller->GetID() == controllerId && controller->IsPressed(static_cast<int>(controllerKey)))
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
				input.HandleJoystick(isLeft, actualCommand, state, deltaTime);
			}
		}
	}
}
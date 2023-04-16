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

class dae::InputManager::InputManagerImpl final
{
	const Uint8* m_pKeyboardState{};

public:

	InputManagerImpl() = default;
	InputManagerImpl(InputManager const& other) = delete;
	InputManagerImpl(InputManagerImpl&& other) = delete;
	InputManagerImpl& operator=(InputManagerImpl const& other) = delete;
	InputManagerImpl& operator=(InputManagerImpl&& other) = delete;
	~InputManagerImpl() = default;

	bool IsPressed(const SDL_Scancode& keyboardKey);
	void ProcessKeyboardInputPressed(float deltaTime, SDL_Scancode& e);
	void ProcessKeyboardInputUp(float deltaTime, SDL_Scancode& e);
	void ProcessKeyboardInputDown(float deltaTime, SDL_Scancode& e);
	void ProcessControllerInput(float deltaTime);
	bool ProcessInput(float deltaTime);
};

bool dae::InputManager::ProcessInput(float deltaTime)
{
	return m_pImpl->ProcessInput(deltaTime);
}

dae::InputManager::InputManager() : m_pImpl{ new dae::InputManager::InputManagerImpl{} }
{
}

dae::InputManager::~InputManager()
{
	delete m_pImpl;
}

void dae::InputManager::ProcessKeyboardInputPressed(float deltaTime, SDL_Scancode& e)
{
	m_pImpl->ProcessKeyboardInputPressed(deltaTime, e);
}

void dae::InputManager::ProcessKeyboardInputUp(float deltaTime, SDL_Scancode& e)
{
	return m_pImpl->ProcessKeyboardInputUp(deltaTime, e);
}

void dae::InputManager::ProcessKeyboardInputDown(float deltaTime, SDL_Scancode& e)
{
	m_pImpl->ProcessKeyboardInputDown(deltaTime, e);
}

void dae::InputManager::ProcessControllerInput(float deltaTime)
{
	return m_pImpl->ProcessControllerInput(deltaTime);
}

bool dae::InputManager::InputManagerImpl::IsPressed(const SDL_Scancode& keyboardKey)
{
	return m_pKeyboardState[keyboardKey];
}

void dae::InputManager::InputManagerImpl::ProcessKeyboardInputPressed(float deltaTime, SDL_Scancode& e)
{
	auto& input = Input::GetInstance();
	auto& commands = input.GetCommands();
	for (auto& command : commands)
	{
		const KeyInput keyInput = command.first;
		if (e == keyInput.scancode && keyInput.state == KeyState::pressed && IsPressed(keyInput.scancode))
		{
			command.second->Execute(deltaTime);
		}
	}
}

void dae::InputManager::InputManagerImpl::ProcessKeyboardInputUp(float deltaTime, SDL_Scancode& e)
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

void dae::InputManager::InputManagerImpl::ProcessKeyboardInputDown(float deltaTime, SDL_Scancode& e)
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

void dae::InputManager::InputManagerImpl::ProcessControllerInput(float deltaTime)
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
			const bool isPressed{ controller->IsPressed(static_cast<int>(controllerKey)) && command.first.state == dae::KeyState::pressed };
			const bool isDown{ controller->IsButtonDown(static_cast<int>(controllerKey)) && command.first.state == dae::KeyState::down };
			const bool isUp{ controller->IsUpThisFrame(static_cast<int>(controllerKey)) && command.first.state == dae::KeyState::up };
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
				input.HandleJoystick(isLeft, actualCommand, state, deltaTime);
			}
		}
	}
}

bool dae::InputManager::InputManagerImpl::ProcessInput(float deltaTime)
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
		ProcessKeyboardInputPressed(deltaTime, e.key.keysym.scancode);
		ImGui_ImplSDL2_ProcessEvent(&e);
	}
	ProcessControllerInput(deltaTime);

	return true;
}

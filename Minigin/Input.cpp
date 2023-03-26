#include "Input.h"
#define WIN32_LEAN_AND_MEAN


void dae::Input::AddController(unsigned int id)
{
	m_Controllers.emplace_back(std::make_unique<XboxController>(id));
}

void dae::Input::BindButtonsToCommand(unsigned int id, XboxController::ControllerButton& button, SDL_Scancode& keyboardButton, KeyState& state, Command* command)
{
	KeyInput input{};
	input.id = static_cast<int>(m_Commands.size());
	input.controllerId = id;
	input.controllerButton = button;
	input.scancode = keyboardButton;
	input.state = state;
	m_Commands.insert({ input, std::unique_ptr<Command>(command) });
}

void dae::Input::BindJoystickToCommand(unsigned int controllerId, Command* command, bool isLeft)
{
	int joystickIdx = isLeft ? 0 : 1;
	JoystickKey key = JoystickKey(controllerId, joystickIdx);
	m_JoystickCommands.insert({ key, std::unique_ptr<Command>(command) });
}

void dae::Input::UpdateControls()
{
	for (auto& controller : m_Controllers)
	{
		controller->Update();
	}
}

void dae::Input::HandleJoystick(bool isLeft, Command* command, XINPUT_STATE state, float deltaTime)
{
	//determine deadzone based on whether the left or right joystick is used
	int deadzone = isLeft ? XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE : XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	glm::vec3 dir{};
	dir.x = isLeft ? state.Gamepad.sThumbLX : state.Gamepad.sThumbRX;
	dir.y = isLeft ? state.Gamepad.sThumbLY : state.Gamepad.sThumbRY;

	//determine how far the controller is pushed. Tried to use it without, but for unit vectors, I could not find a way without square roots
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
	command->Execute(dir, deltaTime);
}
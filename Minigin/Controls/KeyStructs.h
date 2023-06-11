#pragma once
#include "XboxController.h"
enum class KeyState : int {
	up,
	down,
	pressed
};
struct KeyInput {
	int id{};
	unsigned int controllerId{};
	dae::XboxController::ControllerButton controllerButton{};
	SDL_Scancode scancode{};
	KeyState state{};

	bool operator<(const KeyInput& rhs) const
	{
		return this->id < rhs.id;
	}
};
#include "Singleton.h"
#include "Command.h"

namespace dae
{

	class InputManager final : public Singleton<InputManager>
	{
		const Uint8* m_pKeyboardState{};
	public:
		bool IsPressed(const SDL_Scancode& keyboardKey);
		void ProcessKeyboardInputPressed(float deltaTime);
		void ProcessKeyboardInputUp(float deltaTime, SDL_Scancode& e);
		void ProcessKeyboardInputDown(float deltaTime, SDL_Scancode& e);
		bool ProcessInput(float deltaTime);
		void ProcessControllerInput(float deltaTime);
	};

}

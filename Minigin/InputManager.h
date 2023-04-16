#include "Singleton.h"
#include "Command.h"
#include <map>

namespace dae
{

	class InputManager final : public Singleton<InputManager>
	{
	public:

		InputManager();
		InputManager(InputManager const& other) = delete;
		InputManager(InputManager&& other) = delete;
		InputManager& operator=(InputManager const& other) = delete;
		InputManager& operator=(InputManager&& other) = delete;

		~InputManager();
		void ProcessKeyboardInputPressed(float deltaTime, SDL_Scancode& e);
		void ProcessKeyboardInputUp(float deltaTime, SDL_Scancode& e);
		void ProcessKeyboardInputDown(float deltaTime, SDL_Scancode& e);
		bool ProcessInput(float deltaTime);
		void ProcessControllerInput(float deltaTime);

	private:
		class InputManagerImpl;
		InputManagerImpl* m_pImpl{ nullptr };
	};

}

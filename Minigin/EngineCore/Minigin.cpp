#include <windows.h>
#include <stdexcept>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"
#include "../Managers/InputManager.h"
#include "../Managers/SceneManager.h"
#include "../Managers/ResourceManager.h"
#include "../Audio/Audio.h"
#include "../Misc/Locator.h"
#include "../Misc/Logger.h"
#include "Renderer.h"
#include <chrono>
#include <thread>


SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);

	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::Minigin::Minigin(const std::string &dataPath)
{
	PrintSDLVersion();
	
	MG_ASSERT(SDL_Init(SDL_INIT_VIDEO) == 0, "Cannot initialise SDL_Video!!");

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		720,
		630,
		SDL_WINDOW_OPENGL
	);
	MG_ASSERT(g_window != nullptr,"Cannot create SDL_Window");

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);

	Locator::Provide(std::make_unique<Audio>());
}

dae::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	load();
	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& inputManager = InputManager::GetInstance();

	bool doContinue = true;
	auto lastTime = std::chrono::high_resolution_clock::now();
	//float lag{ 0.0f };
	float desiredFrameRate{ 144.f };
	//const float fixedTimeStep{ 2.f };
	int msPerFrame{ static_cast<int>(1000/desiredFrameRate)};
	while (doContinue)
	{
		const auto currentTime = std::chrono::high_resolution_clock::now();
		const float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
		doContinue = inputManager.ProcessInput(deltaTime);
		inputManager.UpdateControls();
/*		lag += deltaTime;
		if (lag >= fixedTimeStep)
		{
			sceneManager.FixedUpdate(fixedTimeStep);
			lag -= fixedTimeStep;
		}*/
		sceneManager.Update(deltaTime);
		renderer.Render();

		const auto sleepTime = currentTime + std::chrono::milliseconds(msPerFrame)
			- std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(sleepTime);

		lastTime = currentTime;
	}
}

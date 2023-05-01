#include <windows.h>
#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"
#include "InputManager.h"
#include "Input.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <chrono>
#include <thread>
#include "Logger.h"


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
	Logger::LogInfo("Creating engine");
	PrintSDLVersion();
	
	MG_ASSERT(SDL_Init(SDL_INIT_VIDEO) == 0);

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		560,
		SDL_WINDOW_OPENGL
	);
	MG_ASSERT(g_window != nullptr);

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);
	Logger::LogInfo("Engine initialised!!");
}

dae::Minigin::~Minigin()
{
	Logger::LogInfo("Destroying engine!!");
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
	Logger::LogInfo("Engine destroyed!!");
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	Logger::LogInfo("Engine loading!!");
	load();
	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& inputManager = InputManager::GetInstance();
	auto& input = Input::GetInstance();

	bool doContinue = true;
	auto lastTime = std::chrono::high_resolution_clock::now();
	//float lag{ 0.0f };
	float desiredFrameRate{ 144.f };
	//const float fixedTimeStep{ 2.f };
	int msPerFrame{ static_cast<int>(1000/desiredFrameRate)};
	Logger::LogInfo("Engine running!!");
	while (doContinue)
	{
		const auto currentTime = std::chrono::high_resolution_clock::now();
		const float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
		doContinue = inputManager.ProcessInput(deltaTime);
		input.UpdateControls();
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

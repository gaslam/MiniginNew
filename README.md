# Minigin

Minigin is a very small project using [SDL2](https://www.libsdl.org/) and [glm](https://github.com/g-truc/glm) for 2D c++ game projects. It is in no way a game engine, only a barebone start project where everything sdl related has been set up. It contains glm for vector math, to aleviate the need to write custom vector and matrix classes.

[![Build Status](https://github.com/avadae/minigin/actions/workflows/msbuild.yml/badge.svg)](https://github.com/avadae/msbuild/actions)
[![GitHub Release](https://img.shields.io/github/v/release/avadae/minigin?logo=github&sort=semver)](https://github.com/avadae/minigin/releases/latest)

# Goal

Minigin can/may be used as a start project for the exam assignment in the course 'Programming 4' at DAE. In that assignment students need to recreate a popular 80's arcade game with a game engine they need to program themselves. During the course we discuss several game programming patterns, using the book '[Game Programming Patterns](https://gameprogrammingpatterns.com/)' by Robert Nystrom as reading material. These patterns need to be applied (when applicable) as well as all (most) [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines).

# Disclaimer

Minigin is, despite perhaps the suggestion in its name, not a game engine. It is just a very simple sdl2 ready project. None of the patterns discussed in the course are used (except singleton which use we challenge during the course). It is up to the students to implement their own vision for their engine, apply patterns as they see fit, create their game as efficient as possible.

# Use

Download the latest release of this project and compile/run in visual studio. Since students need to have their work on github too, they can use this repository as a template.

# File format:


A custom format called BurgerTimeFile is used to load the level data of the game. You can find the files [here](Data/LevelData)


# structure

The project that I had to make contains the following decisions/structure I decided to go with:

## Patterns
The following patterns from the course were implemented:
- [The Locator](Minigin/Misc/Locator.h)
- [The Command](Minigin/Command/Command.h)
- [The Observer](Minigin/Misc/Observer.h)
- [The Game Object](Minigin/Misc/GameObject.h)
- [The States](BurgerTime/States/)

## Components

Every component inherits from this base class

```cpp
namespace dae {
	class GameObject;
	class Component {
	public:
		virtual ~Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) noexcept = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) noexcept = delete;
		virtual void Render() const {};
		virtual void Update(float) {};
		virtual void RenderImGUI() {};
		virtual void Start(){}
		virtual void End(){}

	protected:
		GameObject* GetOwner() const { return m_pOwner; }
		explicit Component(GameObject* owner) : m_pOwner{ owner } {
			MG_ASSERT(owner != nullptr,"Cannot get owner!!");
		};
	private:
		GameObject* m_pOwner{ nullptr };
	};
```

This contains all the functions you expect from a component. It has virtual update and render functions that the derived classes can override for it's own use. It also has an owner that the derived classes can access via the GetOwner() function. Every time a scene starts or ends, the Start/End function gets called to do whatever needs to be changed when the scene becomes active/inactive.

### [Animation Component](BurgerTime/Components/AnimationComponent.h)

This component is responsible for showing sprite animations. It contains some variables and functions that allow the programmer to set the right frames for the animations. The chosen frame is set to show an animation instead of the entire spritesheet as a whole. It also allows for frames to be locked if needed. All of this is perfect for creating multiple animations with just one spritesheet.

### [Enemy Component](BurgerTime/Components/EnemyComponent.h)
The main part of this component is also a Base class. From there, every enemy can inherit from this class. Allowing it to change it's variables or add any extra components like an animation component. This allows extra enemy components to be created with ease.

### [Ladder](BurgerTime/Components/LadderComponent.h) and [Platform components](BurgerTime/Components/PlatformComponent.h)

Both are classes with the same purpose. Once the player enters them, the player can either move up/down or left/right. It also make sure the player doesn't go outside of the boundries for both shapes. Based on whether it is a ladder or platform, the appropriate movement states get set on the player character.

### [Audio Component](BurgerTime/Components/AudioComponent.h)
A decorator service that allows for sending events to the [Audio class](Minigin/Audio/Audio.h). This uses the SDL_Mixer library for playing sound.

### [Character Component](BurgerTime/Components/CharacterComponent.h)

This is where all the logic for the character happens. The code for the movement, the character animations and the states are all in this one component. To improve it, I can definitely look into better ways for creating controller classes for characters.

### [Rigidbody Component](BurgerTime/Components/RigidBodyComponent.h)

Just like every engine, it needs a rigidbody component for collision detection, but also for triggers (not available yet). It relies on the Shape class which uses polymorphism to avoid type switching([see this rule of the C++ coding standards](https://www.oreilly.com/library/view/c-coding-standards/0321113586/ch91.html)). What it does is setting the physical properties of the RigidBody like can it move (m_IsStatic), it's velocity or whether it can fall through the ground.

### [Grid Component](BurgerTime/Components/GridComponent.h)

A grid consisting out of Nodes that act as it's cells. For navigation, the [A*](https://web.archive.org/web/20171022224528/http://www.policyalmanac.org:80/games/aStarTutorial.htm) path finding algorithm is being used to make sure the enemy goes to the player in this grid.

### [Burger Component](BurgerTime/Components/BurgerComponent.h)

All the burger parts need their component in order to function as they should. This class contains all the code for setting the correct states according to the [State pattern with a Finite State Machine](https://gameprogrammingpatterns.com/state.html)

### [Level Component](BurgerTime/Components/LevelComponent.h)

All it does is handle the background music for now, but this is a component that is supposed to handle the state of the level. Is the Game over? Does the level need to restart? This is what I'm trying to reach with that class.

## Managers

All of them are Singletons that can be acessed globally to handle code that is good to be accessed globally

### [Character Manager](BurgerTime/Managers/CharacterManager.h)

A class to handle all character types including the player to allow for easy global access wherever the the game needs them.

### [Scene Manager](Minigin/Managers/SceneManager.h)

I decided to expand this class from the starting project to allow switching between different scene. Before the change, all scenes were rendered and updated together. I made sure to add a number that keeps track of which scene is currently active.

### [Input Manager](Minigin/Managers/InputManager.h)

This is a class that handles all the input and uses the [Keyboard](Minigin/Controls/Keyboard.cpp) and [Xbox controller](Minigin/Controls/XboxController.cpp) classes that constantly update keyboard/controller states in their own way to detect changes in the button presses of both things. To allow for more options, I made sure to add options to check if a button is pressed, down or up via it's own functions. Here is an [example](BurgerTime/States/MovingState.cpp) of a use case:

```cpp
	const bool isLeftKeyDown{ manager.GetInputKeyDown("left") };
	const bool isRightKeyDown{ manager.GetInputKeyDown("right") };
	const bool isUpKeyDown{ manager.GetInputKeyDown("up") };
	const bool isDownKeyDown{ manager.GetInputKeyDown("down") };

	if(isLeftKeyDown)
	{
		m_Dir.x = -1.f;
	}
	if (isRightKeyDown)
	{
		m_Dir.x = 1.f;
	}

	if(isUpKeyDown)
	{
		m_Dir.y = -1.f;
	}

	if (isDownKeyDown)
	{
		m_Dir.y = 1.f;
	}
```

But that is not the only option. The Input can also be triggered via commands in cases where code is not executed over and over and over again. For example, in my [Utils.h](BurgerTime/Utils.h) of the BurgerTime game, I use commands to switch levels: 

```cpp
	inline void AddScenes()
	{
		auto player{ dae::CharacterManager::GetInstance().InitPlayer() };
		AddScene(1,player);
		AddScene(2,player);
		AddScene(3,player);

		dae::SceneManager::GetInstance().SetScene(0);
		bool moveToNext{false};
		constexpr int controllerIndex{ 0 };
		auto controllerButton{ dae::XboxController::ControllerButton::LeftShoulder };
		SDL_Scancode keyboardButton{ SDL_SCANCODE_F3 };
		auto keyState{ KeyState::down };
		dae::InputManager::GetInstance().BindButtonsToCommand(controllerIndex, controllerButton, keyboardButton, keyState, new dae::SceneSwitchCommand{ moveToNext });
		moveToNext = true;
		controllerButton = dae::XboxController::ControllerButton::RightShoulder;
		keyboardButton = SDL_SCANCODE_F4;
		dae::InputManager::GetInstance().BindButtonsToCommand(controllerIndex, controllerButton, keyboardButton, keyState, new dae::SceneSwitchCommand{ moveToNext });
	}
```

## Audio

You can find everything related to audio [here](Minigin/Audio/).

The [audio base class](Minigin/Audio/AudioBase.h) is a class that various potential audio services can inherit from. In this case SDL_Mixer is used. The [audio class](Minigin/Audio/AudioBase.h) receives events from components like the [Audio component](BurgerTime/Components/AudioComponent.cpp), adds them to an event queue/buffer which handles the event on a seperate thread to allow for audio to run independently from the game.


## [Shape](Minigin/Misc/Shape.h)

This is a simple interface containing functions like IsPointIn and Collides with to allow classes to inherit from it add their own implementation of the functions. For example, the [RectangleShape class](Minigin/Misc/Shape.h)

## Project utils in [BurgerTime](BurgerTime/Utils.h) and [Minigin](Minigin/Utils/Utils.h)

Contains functions for loading level data or that can be useful anywhere without needing a class. For example: comparing floats.
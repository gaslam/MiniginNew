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


# structure

The project that I had to make contains the following decisions/structure I decided to go with:

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

This contains all the functions you expect from a component. It has virtual update and render functions that the derived classes can override for it's own use. It also has an owner that the derived classes can access via the GetOwner() function. Every time a scene starts or ends, the Start/End function gets called to do whatever needs to be changes when the scene becomes active/inactive.

### [Animation Component](BurgerTime/Components/AnimationComponent.h)

This component is responsible for showing sprite animations. It contains some variables and functions that allow the programmer to set the right frames for the animations. It just changes the frame to show an animation instead of the entire sprite-sheet as a whole. It also allows for frames to be locked if needed. All of this is perfect for creating multiple animations with just one spritesheet.

### [Enemy Component](BurgerTime/Components/EnemyComponent.h)
The main part of this component is also a Base class. From there on every enemy enemy can inherit from this class. Allowing it to change it's variables or add any extra components like an animation component. This allows extra enemie components to be created with ease.

### [Ladder](BurgerTime/Components/LadderComponent.h) and [Platform components](BurgerTime/Components/PlatformComponent.h)

Both are classes with the same purpose. Once the player enters them it can either move up/down or left/right. It also make sure the player doesn't go outside of the boundries for both shapes. Based on whether it is a ladder or platform, the appropriate movement states get set on the player character.

### [Audio Component](BurgerTime/Components/AudioComponent.h)
A decorator service that allows for sending events to the [Audio class](Minigin/Audio/Audio.h). This uses the SDL_Mixer library for playing sound

### [Character Component](BurgerTime/Components/CharacterComponent.h)

This is where all the logic for the character happens. The code for the movement, the character animations and the states are all in this one component. To improve it, I can definitely look into better ways for creating controller classes for characters.

### [Rigidbody Component](BurgerTime/Components/RigidBodyComponent.h)

Just like every engine, it needs a rigidbody component for collision detection, but also for triggers (not available yet). It relies on the Shape class which uses polymorphism to avoid type switching([see this rule of the C++ coding standards](https://www.oreilly.com/library/view/c-coding-standards/0321113586/ch91.html)). What it does is setting the physical properties of the RigidBody like can it move (m_IsStatic), it's velocity or whether it can fall through the ground.

### [Grid Component](BurgerTime/Components/GridComponent.h)

A grid consisting out of Nodes that act as it's cells. For navigation, the [A*](https://web.archive.org/web/20171022224528/http://www.policyalmanac.org:80/games/aStarTutorial.htm) path finding algorithm is being used to make sure the enemy goes to the player in this grid.

### [Burger Component](BurgerTime/Components/BurgerComponent.h)
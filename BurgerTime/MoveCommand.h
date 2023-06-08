#pragma once
#include <Command.h>
namespace dae
{
	class MoveCommand : public GameObjectCommand {
		glm::vec2 m_Dir{};
	public:

		explicit MoveCommand(GameObject* owner) : GameObjectCommand(owner) {};
		MoveCommand(GameObject* owner, glm::vec2& dir) : GameObjectCommand(owner), m_Dir{ dir } {};
		void Execute(float deltaTime) override;
		void Execute(glm::vec2& dir, float deltaTime) override;
	};
}


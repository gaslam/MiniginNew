#pragma once
#include <Command.h>
namespace dae
{
	class MoveCommand : public GameObjectCommand {
		glm::vec3 m_Dir{};
	public:

		explicit MoveCommand(GameObject* owner) : GameObjectCommand(owner) {};
		MoveCommand(GameObject* owner, glm::vec3& dir) : GameObjectCommand(owner), m_Dir{ dir } {};
		void Execute(float deltaTime) override;
		void Execute(glm::vec3& dir, float deltaTime) override;

		~MoveCommand() override = default;
		MoveCommand(const MoveCommand& other) = delete;
		MoveCommand(MoveCommand&& other) noexcept = delete;
		MoveCommand& operator=(const MoveCommand& other) = delete;
		MoveCommand&& operator=(MoveCommand&& other) noexcept = delete;
	};
}


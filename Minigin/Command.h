#pragma once
#include <glm/glm.hpp>
namespace dae {
	class GameObject;
	class Command {
	public:
		virtual ~Command() = default;
		Command(const Command& other) = delete;
		Command(Command&& other) noexcept = delete;
		Command& operator=(const Command& other) = delete;
		Command&& operator=(Command&& other) noexcept = delete;
		explicit Command() = default;
		//float = deltaTime
		virtual void Execute(float) {};
		//float = deltaTime
		virtual void Execute(glm::vec3&, float) {};
	};


	class GameObjectCommand : public Command {
		GameObject* m_Owner;
	protected:
		GameObject* GetCommandOwner() const { return m_Owner; }
	public:
		explicit GameObjectCommand(GameObject* owner) : m_Owner(owner){};
		~GameObjectCommand() override = default;
		GameObjectCommand(const GameObjectCommand& other) = delete;
		GameObjectCommand(GameObjectCommand&& other) noexcept = delete;
		GameObjectCommand& operator=(const GameObjectCommand& other) = delete;
		GameObjectCommand&& operator=(GameObjectCommand&& other) noexcept = delete;
	};

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

	class SetChacterToStateIdleCommand : public GameObjectCommand {
	public:
		explicit SetChacterToStateIdleCommand(GameObject* owner) : GameObjectCommand(owner) {};
		void Execute(float) override;

		~SetChacterToStateIdleCommand() override = default;
		SetChacterToStateIdleCommand(const SetChacterToStateIdleCommand& other) = delete;
		SetChacterToStateIdleCommand(SetChacterToStateIdleCommand&& other) noexcept = delete;
		SetChacterToStateIdleCommand& operator=(const SetChacterToStateIdleCommand& other) = delete;
		SetChacterToStateIdleCommand&& operator=(SetChacterToStateIdleCommand&& other) noexcept = delete;
	};
}

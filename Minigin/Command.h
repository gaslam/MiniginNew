#pragma once
#include <glm/glm.hpp>
namespace dae {
	class GameObject;
	class Command {
	public:
		Command() = default;
		//float = deltaTime
		virtual void Execute(float) {};
		//float = deltaTime
		virtual void Execute(glm::vec3&,float) {};
	};


	class GameObjectCommand: public Command{
		GameObject* m_Owner;
	protected:
		GameObject* GetCommandOwner() const { return m_Owner; }
	public:
		GameObjectCommand(GameObject* owner);
	};


	class DamageCommand : public GameObjectCommand {
	public:
		DamageCommand(GameObject* owner) : GameObjectCommand(owner) {};
		virtual void Execute(float) override;
	};

	class ScoreCommand : public GameObjectCommand {
		int m_ScoreToAdd = 1;
	public:
		ScoreCommand(GameObject* owner) : GameObjectCommand(owner) {};
		virtual void Execute(float) override;
	};
	class MoveCommand : public GameObjectCommand {
		glm::vec3 m_Dir{};
	public:
		MoveCommand(GameObject* owner) : GameObjectCommand(owner), m_Dir{} {};
		MoveCommand(GameObject* owner, glm::vec3& dir) : GameObjectCommand(owner), m_Dir{ dir } {};
		virtual void Execute(float deltaTime) override;
		virtual void Execute(glm::vec3& dir, float deltaTime) override;
	};
}

#pragma once
#include <glm/glm.hpp>
namespace dae {
	class GameObject;
	class Command {
		GameObject* m_Owner;
	protected:
	GameObject* GetCommandOwner() const { return m_Owner; }
	public:
		Command(GameObject* actor);
		//float = deltaTime
		virtual void Execute(float) {};
		//float = deltaTime
		virtual void Execute(glm::vec3&,float) {};
	};

	class MoveCommand : public Command {
		glm::vec3 m_Dir{};
	public:
		explicit MoveCommand(GameObject* owner) : Command(owner), m_Dir{} {};
		MoveCommand(GameObject* owner, glm::vec3& dir) : Command(owner), m_Dir{ dir } {};
		virtual void Execute(float deltaTime) override;
		virtual void Execute(glm::vec3& dir, float deltaTime) override;
	};

	class SetChacterToStateIdleCommand : public Command {
	public:
		explicit SetChacterToStateIdleCommand(GameObject* owner) : Command(owner) {};
		virtual void Execute(float) override;
	};
}

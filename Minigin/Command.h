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
		virtual void Execute() {};
		//float 1 = x, float 2 = y
		virtual void Execute(float, float) {};
	};

	class MoveCommand : public Command {
		glm::vec3 m_Dir{};
	public:
		MoveCommand(GameObject* owner) : Command(owner), m_Dir{} {};
		MoveCommand(GameObject* owner, glm::vec3& dir) : Command(owner), m_Dir{dir} {};
		virtual void Execute() override;
		virtual void Execute(float x, float y) override;
	};
}

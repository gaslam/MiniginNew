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
		Command& operator=(Command&& other) noexcept = delete;
		explicit Command() = default;
		//float = deltaTime
		virtual void Execute(float) {};
		//float = deltaTime
		virtual void Execute(glm::vec2&, float) {};
	};


	class GameObjectCommand : public Command {
		GameObject* m_Owner;
	protected:
		GameObject* GetCommandOwner() const { return m_Owner; }
	public:
		explicit GameObjectCommand(GameObject* owner) : m_Owner(owner){};
		virtual ~GameObjectCommand() override = default;
		GameObjectCommand(const GameObjectCommand& other) = delete;
		GameObjectCommand(GameObjectCommand&& other) noexcept = delete;
		GameObjectCommand& operator=(const GameObjectCommand& other) = delete;
		GameObjectCommand& operator=(GameObjectCommand&& other) noexcept = delete;
	};
}

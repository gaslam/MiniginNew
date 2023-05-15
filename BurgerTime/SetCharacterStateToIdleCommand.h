#pragma once
#include <Command.h>
namespace dae
{
	class SetCharacterStateToIdleCommand :
		public GameObjectCommand
	{
	public:
		explicit SetCharacterStateToIdleCommand(GameObject* owner) : GameObjectCommand(owner) {}
		void Execute(float) override;
		void Execute(glm::vec3&, float) override{}
	};
}


#pragma once
#include <Command.h>
namespace dae
{
	class SetCharacterStateToIdleCommand :
		public GameObjectCommand
	{
	public:
		explicit SetCharacterStateToIdleCommand(GameObject* owner) : GameObjectCommand(owner) {};
		void Execute(float) override;

		~SetCharacterStateToIdleCommand() override = default;
		SetCharacterStateToIdleCommand(const SetCharacterStateToIdleCommand& other) = delete;
		SetCharacterStateToIdleCommand(SetCharacterStateToIdleCommand&& other) noexcept = delete;
		SetCharacterStateToIdleCommand& operator=(const SetCharacterStateToIdleCommand& other) = delete;
		SetCharacterStateToIdleCommand&& operator=(SetCharacterStateToIdleCommand&& other) noexcept = delete;
	};
}


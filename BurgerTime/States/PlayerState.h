#pragma once

namespace dae
{
	class CharacterComponent;
	class CharacterState
	{
	public:
		explicit CharacterState() = default;
		virtual ~CharacterState() = default;

		CharacterState(const CharacterState& other) = delete;
		CharacterState(CharacterState&& other) noexcept = delete;
		CharacterState& operator=(const CharacterState& other) = delete;
		CharacterState& operator=(CharacterState&& other) noexcept = delete;
		virtual void Update(CharacterComponent* pComponent, float deltaTime) = 0;
		virtual CharacterState* HandleInput() = 0;
		virtual void OnEnter(CharacterComponent*) {};
		virtual void OnExit(CharacterComponent*) {};
	};
}




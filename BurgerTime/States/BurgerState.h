#pragma once
namespace dae
{
	class BurgerComponent;
	class BurgerState
	{
	public:
		explicit BurgerState() = default;
		virtual ~BurgerState() = default;

		BurgerState(const BurgerState& other) = delete;
		BurgerState(BurgerState&& other) noexcept = delete;
		BurgerState& operator=(const BurgerState& other) = delete;
		BurgerState& operator=(BurgerState&& other) noexcept = delete;
		virtual void Update(BurgerComponent* pComponent, float deltaTime) = 0;
		virtual BurgerState* HandleInput() = 0;
		virtual void OnEnter(BurgerComponent*) {};
		virtual void OnExit(BurgerComponent*) {};
	};

}


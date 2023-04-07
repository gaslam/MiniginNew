#pragma once
class Event;
namespace dae {
	class GameObject;
	class Observer
	{
	public:
		Observer() = default;
		virtual ~Observer() {};
		virtual void Notify(GameObject* object, Event& event) = 0;
	};

	class PlayerDied : Observer {
	public:
		void Notify(Event& event, GameObject* object);
	private:
		void DisplayGameOver(GameObject* object);
	};

	class PlayerDamaged : Observer {
	public:
		void Notify(Event& event, GameObject* object);
	private:
		void UpdateHealthDisplay(GameObject* object);
	};
}


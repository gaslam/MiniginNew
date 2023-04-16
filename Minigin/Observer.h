#pragma once
#include <memory>
#include "SteamArchievements.h"

class Event;
namespace dae {
	class GameObject;
	class Observer
	{
	public:
		explicit Observer() = default;
		virtual ~Observer() = default;

		Observer(const Observer& other) = delete;
		Observer(Observer&& other) = delete;
		Observer& operator=(const Observer& other) = delete;
		Observer& operator=(Observer&& other) = delete;
		virtual void OnNotify(GameObject* object, Event& event) = 0;
	};

	class PlayerObserver : public Observer {
	public:
		PlayerObserver() = default;
		void OnNotify(GameObject* object, Event& event) override;
		void UpdateScore(GameObject* object);
	private:
		void DisplayGameOver(GameObject* object);
		void UpdateHealthDisplay(GameObject* object);
	};

	class AchievementObserver : public Observer {
	public:
		AchievementObserver() = default;
		void OnNotify(GameObject* object, Event& event) override;
	private:
		void CheckScore(GameObject* object,SteamArchievements& achievements);
	};
}


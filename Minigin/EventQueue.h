#pragma once
#include <unordered_map>
#include "Event.h"
#include <functional>
namespace dae {
	class EventQueue
	{
		void AddEvent(const Event& e);
		void AddListener(const Event& e, std::function<void()>& value);
		void NotifyListeners();
	private:
		std::unordered_map<Event, std::vector<std::function<void()>>> m_Listeners{};
		int m_EventBufferStart{};
		int m_EventsQueued{};
		static const int m_EventBufferSize{ 8 };
		Event events[m_EventBufferSize];
	};
}


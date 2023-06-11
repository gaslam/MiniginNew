#pragma once
#include <vector>
#include "Observer.h"

//Made with the help of a former student.
//Was originally used with std::function, but I had difficulties implementing the removeListener. Look for that in my previous submission
namespace dae {
	class Subject
	{
	public:
		void AddObserver(std::shared_ptr<Observer> pObserver)
		{
			m_Callbacks.push_back(pObserver);
		}

		void RemoveObserver(std::shared_ptr<Observer> pObserver)
		{
			auto it = std::remove(m_Callbacks.begin(), m_Callbacks.end(), pObserver);
			if (it != m_Callbacks.end())
			{
				m_Callbacks.erase(it);
			}
		}

		void Invoke(GameObject* object,Event& event)
		{
			for (auto& observer : m_Callbacks)
			{
				observer->OnNotify(object, event);
			}
		}

	private:
		std::vector<std::shared_ptr<Observer>> m_Callbacks{};
	};
}


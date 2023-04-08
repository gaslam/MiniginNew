#pragma once
#include <vector>
#include "Singleton.h"
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <algorithm>
#include "Observer.h"
namespace dae {
	class ObserverManager : public Singleton<ObserverManager>
	{
	public:

		template <typename T, typename... Args> T* AddObserver(Args&&... args) {
			if (IsObserverAdded<T>()) {
				return GetObserver<T>();
			}

			const std::type_index typeIndex = std::type_index(typeid(T));
			auto observer = std::make_unique<T>(std::forward<Args>(args)...);
			auto pointer = dynamic_cast<T*>(observer.get());

			m_Observers.emplace(typeIndex, std::move(observer));

			return pointer;
		};

		template<typename T>
		T* GetObserver() const
		{
			const std::type_index typeIndex = std::type_index(typeid(T));
			if (!IsObserverAdded<T>())
			{
				return nullptr;
			}
			auto observer = dynamic_cast<T*>(m_Observers.at(typeIndex).get());
			return observer;
		}
		template<typename T>
		T* RemoveObserver()
		{
			const std::type_index typeIndex = std::type_index(typeid(T));
			auto observer = m_Observers.at(typeIndex);

			if (observer)
			{
				m_Observers.erase(typeIndex);
			}
		}

		template<typename T>
		bool IsObserverAdded() const
		{
			const std::type_index typeIndex = std::type_index(typeid(T));

			return m_Observers.contains(typeIndex);
		}
	private:
		std::unordered_map<std::type_index, std::unique_ptr<Observer>> m_Observers{};
	};
}


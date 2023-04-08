#pragma once
#include <vector>
#include <functional>

//Made with the help of a former student.
template<typename... T>
class Delegate final
{
public:
	using CallBack = std::function<void(T...)>;

	void AddListener(const CallBack& fn)
	{
		m_Callbacks.push_back(fn);
	}

	void AddListener(CallBack&& fn)
	{
		m_Callbacks.push_back(std::move(fn));
	}



	template <typename... Args>
	void Invoke(Args&& ...args)
	{
		for (const CallBack& fn : m_Callbacks)
		{
			fn(std::forward<Args>(args)...);
		}
	}

private:
	std::vector<CallBack> m_Callbacks{};
};


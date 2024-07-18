#ifndef _EVENT_H_
#define _EVENT_H_
#include "EventCallback.h"
#include <vector>

template<typename... Args>
class Event
{
public:
	Event() = default;
	~Event()
	{
		for (IEventCallback<Args...>* action : m_actions)
			delete action;

		m_actions.clear();
	}

	Event(Event& other)
	{
		for (IEventCallback<Args...>* action : other.m_actions)
		{
			m_actions.push_back(action->Clone());
		}
	}

	Event(Event&& other)
	{
		m_actions = std::move(other.m_actions);
		other.m_actions.clear();
	}

	Event& operator=(const Event& other)
	{
		if (this != &other)
		{
			for (IEventCallback<Args...>* action : m_actions)
			{
				delete action;
			}

			m_actions.clear();

			for (IEventCallback<Args...>* action : other.m_actions)
			{
				m_actions.push_back(action->Clone());
			}
		}

		return *this;
	}

	Event& operator=(Event&& other)
	{
		if (this != &other)
		{
			for (IEventCallback<Args...>* action : m_actions)
			{
				delete action;
			}

			m_actions.clear();

			m_actions = std::move(other.m_actions);
			other.m_actions.clear();
		}

		return *this;
	}

	template<typename T, typename... ExtraArgs>
	IEventCallback<Args...>* AddListener(T* instance, void (T::* function)(Args..., ExtraArgs...), const ExtraArgs&... extraArgs )
	{
		EventCallback<T, Args...>* callback = new EventCallback<T, Args...>(instance, function, extraArgs...);
		m_actions.push_back(callback);
		return callback;
	}

	void RemoveListener(IEventCallback<Args...>* action)
	{
		auto position = std::find(m_actions.begin(), m_actions.end(), action);

		if (position == m_actions.end())
			return;

		m_actions.erase(position);


		if(action != nullptr)
			delete action;
	}

	void Trigger(Args... args)
	{
		for (IEventCallback<Args...>* action : m_actions)
		{
			action->Trigger(args...);
		}
	}

private:
	std::vector<IEventCallback<Args...>*> m_actions;
};

#endif

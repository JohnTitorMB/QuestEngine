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

	void AddListener(IEventCallback<Args...>* action)
	{
		m_actions.push_back(action);
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

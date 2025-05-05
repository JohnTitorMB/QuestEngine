#include "TimeManager.h"

TimeManager* TimeManager::m_instance = nullptr;

TimeManager::TimeManager()
{
	m_deltaTime = 0.0f;
	m_time = 0.0;
	m_previousTime = 0.0f;
}

TimeManager* TimeManager::Instance()
{
	if (m_instance == nullptr)
	{
		m_instance = new TimeManager();
	}

	return m_instance;
}


TimeManager::~TimeManager()
{
	for (auto it = m_timers.begin(); it != m_timers.end(); ++it)
	{
		Timer* timer = *it;
		delete timer;
	}

	m_timers.clear();

}

void TimeManager::Destroy()
{
	delete m_instance;
	m_instance = nullptr;
}

float TimeManager::GetDeltaTime() const
{
	return m_deltaTime;
}

float TimeManager::GetTime() const
{
	return m_time;
}

float TimeManager::GetPreviousTime() const
{
	return m_previousTime;
}


Timer& TimeManager::SetTimer(float duration, bool repeat, Event<>& _event)
{
	Timer* timer = new Timer();
	timer->m_duration = duration;
	timer->m_repeat = repeat;
	timer->m_event = _event;

	m_timers.push_back(timer);

	return *timer;
}

void TimeManager::RemoveTimer(Timer& timer)
{
	auto it = std::find(m_timers.begin(), m_timers.end(), &timer);

	if (it != m_timers.end())
	{
		delete *it;
		m_timers.erase(it);
	}
}

void TimeManager::UpdateTimers()
{
	for (auto it = m_timers.begin(); it != m_timers.end(); ++it)
	{
		Timer* timer = *it;
		if (!timer->m_isPause)
		{
			timer->m_time += m_deltaTime;
			if (timer->m_time >= timer->m_duration)
			{
				timer->m_event.Trigger();

				if (timer->m_repeat)
					timer->m_time = 0.0f;
				else
				{
					delete timer;
					m_timers.erase(it);
					--it;
				}
			}
		}
	}
}


Timer::Timer()
{
	m_event = Event();
	m_duration = 0.0f;
	m_time = 0.0f;
	m_repeat = false;
	m_isPause = false;
}


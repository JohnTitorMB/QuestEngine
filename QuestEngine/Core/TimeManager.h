#ifndef _TIMEMANAGER_H_
#define _TIMEMANAGER_H_
#include "World.h"
#include "Events/Event.h"
class Timer
{
public:
	Timer();
	Event<> m_event;
	float m_duration;
	bool m_repeat;
	float m_time;
	bool m_isPause = false;
};

class QuestEngine;
class TimeManager
{
	friend class World;
	friend class QuestEngine;

	TimeManager(TimeManager& other) = delete;
	~TimeManager();

	static void Destroy();

public :
	float GetDeltaTime()const;
	float GetTime()const;
	float GetPreviousTime()const;
	static TimeManager* Instance();

	Timer& SetTimer(float duration, bool repeat, Event<>& _event); //To implement
	void RemoveTimer(Timer& timer); //To implement

private:
	float m_deltaTime;
	float m_time;
	float m_previousTime;

protected:
	static TimeManager* m_instance;
	TimeManager();

	std::vector<Timer*> m_timers;

	void UpdateTimers(); //To implement

};

#endif // !_TIMEMANAGER_H_



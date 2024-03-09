#ifndef _EVENTCALLBACK_H_
#define _EVENTCALLBACK_H_

template<typename... Args>
class IEventCallback
{
public:
	virtual bool operator == (IEventCallback* other) = 0;
	virtual void Trigger(Args... args) = 0;
};
 
template<typename T, typename... Args>
class EventCallback : public IEventCallback<Args...>
{    
public:    
	EventCallback(T* instance, void (T::* function)(Args... args))
	{
		m_instance = instance;
		m_function = function;
	}
	virtual void Trigger(Args... args) override
	{ 
		if(m_instance != nullptr)
			(m_instance->*m_function)(args...);
	} 
	
	virtual bool operator == (IEventCallback<Args...>* other) override
	{
		EventCallback<T, Args...>* otherEventCallback = dynamic_cast<EventCallback<T, Args...>*>(other);
		if (otherEventCallback == nullptr)
			return false;

		return 	(this->m_function == otherEventCallback->m_function) &&
			(this->m_instance == otherEventCallback->m_instance);
	}


private:
	void (T::*m_function)(Args...);
	T* m_instance;
};

#endif



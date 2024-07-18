#ifndef _EVENTCALLBACK_H_
#define _EVENTCALLBACK_H_
#include <iostream>

template<typename... Args>
class ICallback
{
public:
	virtual bool operator == (ICallback* other) = 0;
	virtual void Trigger(Args... args) = 0;
	virtual std::unique_ptr<ICallback<Args...>> Clone() = 0;
};

template<typename... Args>
class IEventCallback
{
public:
	virtual bool operator == (IEventCallback* other) = 0;
	virtual void Trigger(Args... args) = 0;
	virtual IEventCallback<Args...>* Clone() = 0;
};
 
template<typename T, typename... Args>
class EventCallback : public IEventCallback<Args...>
{    
public:    

	EventCallback() 
	{
	
	}

	template<typename... ExtraArgs>
	EventCallback(T* instance, void (T::* function)(Args... args, ExtraArgs... extraArgs), const ExtraArgs&... extraArgs)
	{
		class CallbackWrapper : public ICallback<Args...>
		{
			void (T::* m_function)(Args..., ExtraArgs...);
			T* m_instance;
			std::tuple<ExtraArgs...> m_extraArgs;
			public :
				CallbackWrapper(T* _instance, void (T::* _function)(Args... _args, ExtraArgs... _extraArgs), const ExtraArgs&... _extraArgs)
				{
					m_instance = _instance;
					m_function = _function;
					m_extraArgs = std::make_tuple(_extraArgs...);
				}

				virtual void Trigger(Args... _args) override
				{
					//Expand a tuple into variadic template functions arguments
					std::apply([&](const ExtraArgs&... _extra_args) {
						(m_instance->*m_function)(_args..., _extra_args...);
						}, m_extraArgs);
				}

				virtual bool operator == (ICallback* other) override
				{
					CallbackWrapper* otherCallback = dynamic_cast<CallbackWrapper*>(other);
					if (otherCallback == nullptr)
						return false;

					return 	(this->m_function == otherCallback->m_function) &&
						(this->m_instance == otherCallback->m_instance) && 
						this->m_extraArgs == otherCallback->m_extraArgs;
				}

				virtual std::unique_ptr<ICallback<Args...>> Clone() override
				{
					return std::apply([this](ExtraArgs... unpackedExtraArgs) {
						return std::make_unique<CallbackWrapper>(m_instance, m_function, unpackedExtraArgs...);
						}, m_extraArgs);
				}
		};

		m_callback = std::make_unique<CallbackWrapper>(instance, function, extraArgs...);
	}

	EventCallback(const EventCallback& other)
	{
		if (other.m_callback)
			m_callback = other.m_callback->Clone();
	}

	EventCallback& operator=(const EventCallback& other)
	{
		if (this != &other)
		{
			if (other.m_callback)
			{
				m_callback = other.m_callback->Clone();
			}
			else
			{
				m_callback.reset();
			}
		}
		return *this;
	}

	EventCallback(EventCallback&& other) = default;

	EventCallback& operator=(EventCallback && other) = default;


	virtual void Trigger(Args... args) override
	{ 
		m_callback->Trigger(args...);
	} 
	
	virtual IEventCallback<Args...>* Clone() override
	{
		EventCallback<T, Args...>* eventCallback = new EventCallback<T, Args...>();

		eventCallback->m_callback = m_callback.get()->Clone();

		return eventCallback;
	}

	virtual bool operator == (IEventCallback<Args...>* other) override
	{

		std::cout << "Operator is called" << std::endl;

		EventCallback<T, Args...>* otherEventCallback = dynamic_cast<EventCallback<T, Args...>*>(other);
		if (otherEventCallback == nullptr)
			return false;
			
		return *(this->m_callback) == otherEventCallback->m_callback.get();
	}

private:
	std::unique_ptr<ICallback<Args...>> m_callback;
};

#endif



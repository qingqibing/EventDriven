#pragma once
#include "IEvent.h"

// This class is the interface of the event call back function.
namespace Event
{

template<typename EVENT_TYPE>
class EventDelegate
{
public:
	virtual void invoke(EVENT_TYPE * event) = 0;
};


// the main target of this class is to wrap the caller,
// and make it easy to call target function, just like std::bind.
template<typename RECIEVER_TYPE, typename EVENT_TYPE>
class EventDelegateWrapper
	:public EventDelegate<EVENT_TYPE>
{
	// for short the function name,
	// give it another name.
	typedef void (RECIEVER_TYPE::*CallBackFunction)(EVENT_TYPE *);
public:
	EventDelegateWrapper(RECIEVER_TYPE * reciever, CallBackFunction callBack);
	~EventDelegateWrapper();

private:
	RECIEVER_TYPE * _reciever;
	// the _callBack is a member function pointer of the _reciever.
	CallBackFunction _callBack;
	
	virtual void invoke(EVENT_TYPE * e) override;
	
};

template<typename RECIEVER_TYPE, typename EVENT_TYPE>
inline EventDelegateWrapper<RECIEVER_TYPE, EVENT_TYPE>::EventDelegateWrapper(RECIEVER_TYPE * reciever, CallBackFunction callBack)
	:_reciever(reciever), _callBack(callBack)
{
}

template<typename RECIEVER_TYPE, typename EVENT_TYPE>
inline EventDelegateWrapper<RECIEVER_TYPE, EVENT_TYPE>::~EventDelegateWrapper()
{
}

template<typename RECIEVER_TYPE, typename EVENT_TYPE>
inline void EventDelegateWrapper<RECIEVER_TYPE, EVENT_TYPE>::invoke(EVENT_TYPE * e)
{
	(_reciever->*_callBack)(e);
}

}// namespace Event


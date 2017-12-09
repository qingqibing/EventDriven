#pragma once
#include "IEvent.h"

// This class is the interface of the event call back function.
namespace Event
{

class IEventDelegate
{
public:
	IEventDelegate();
	~IEventDelegate();

	virtual void invoke(IEvent * event) = 0;
};

template<typename RECIEVER_TYPE, typename EVENT_TYPE>
class EventDelegate
	:public IEventDelegate
{
	typedef void (RECIEVER_TYPE::*CallBackFunction)(EVENT_TYPE *);
public:
	EventDelegate(RECIEVER_TYPE * reciever, CallBackFunction callBack);
	~EventDelegate();

private:
	RECIEVER_TYPE * _reciever;
	CallBackFunction _callBack;
	
	virtual void invoke(IEvent * event);
	
};

template<typename RECIEVER_TYPE, typename EVENT_TYPE>
inline EventDelegate<RECIEVER_TYPE, EVENT_TYPE>::EventDelegate(RECIEVER_TYPE * reciever, CallBackFunction callBack)
	:_reciever(reciever), _callBack(callBack)
{
}

template<typename RECIEVER_TYPE, typename EVENT_TYPE>
inline EventDelegate<RECIEVER_TYPE, EVENT_TYPE>::~EventDelegate()
{
}

template<typename RECIEVER_TYPE, typename EVENT_TYPE>
inline void EventDelegate<RECIEVER_TYPE, EVENT_TYPE>::invoke(IEvent * e)
{
	(_reciever->*_callBack)(reinterpret_cast<EVENT_TYPE*>(e));
}

}// namespace Event


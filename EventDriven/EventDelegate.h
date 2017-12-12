#pragma once
#include "TypeTool.h"

namespace Event
{

typedef size_t ID;

struct DelegateID
{
	ID reciever;
	// this pointer is just used to identify different delegate.
	ID pReciever;
	ID event;

	bool operator == (const DelegateID & otherDelegateID) const
	{
		return 
				this->reciever	== otherDelegateID.reciever
			&&	this->pReciever == otherDelegateID.pReciever
			&&	this->event		== otherDelegateID.event;
	}
};

// this struct have no member, 
// it is just used to generator the RecieverID.
struct RecieverSeriesIDSeed {};
typedef TypeTool::IDGenerator<RecieverSeriesIDSeed> RecvIDGenerator;
// this struct have no member,
// it is just used to generator the EventID.
struct EventSeriesIDSeed {};
typedef TypeTool::IDGenerator<EventSeriesIDSeed> EventIDGenerator;


// the interface of the Delegate,
// which will be used to store in the dispatcher<EVENT_TYPE>,
// and the dispathcer, which is a specific place that can dispatch 
// one event type to all the registered delegate.
template<typename EVENT_TYPE>
class EventDelegate
{
public:
	virtual void invoke(EVENT_TYPE * event) = 0;
	virtual DelegateID getID() const = 0;
	bool operator == (const EventDelegate<EVENT_TYPE> & otherDelegate);
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
	virtual DelegateID getID() const override;
	
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
template<typename RECIEVER_TYPE, typename EVENT_TYPE>
inline DelegateID EventDelegateWrapper<RECIEVER_TYPE, EVENT_TYPE>::getID() const
{
	static DelegateID delegateID = 
		{	RecvIDGenerator::newID<RECIEVER_TYPE>(), 
			reinterpret_cast<Event::ID>(_reciever),
			EventIDGenerator::newID<EVENT_TYPE>()};
	return delegateID;
}
template<typename EVENT_TYPE>
inline bool EventDelegate<EVENT_TYPE>::operator==(const EventDelegate<EVENT_TYPE> & otherDelegate)
{
	return (this->getID()) == (otherDelegate.getID());
}
}// namespace Event


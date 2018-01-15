#pragma once
#include "EventDelegate.h"
#include "EventDispatcher.h"
#include <functional>

namespace Event
{

// The argument of the template:
// DERIVED_LISTENER: the class who implement the ListenEvent()function.
// LISTENED_EVENTS: All the events that the listener will listen,
//					basically it is just store the Event types, and make the eventHandler easy to find
//					which type of event the listener care about.
// be aware that there should be no virtual function of the DERIVED_LISTENER,
// or the reinterpret_cast will not work.
template<typename DERIVED_LISTENER, typename...LISTENED_EVENTS>
class EventListener
{
public:
	virtual ~EventListener();

	// get the delegate, 
	// the delegate is on the heap,
	// but the dispatcher will delete it,
	// or the user should take care of it.
	template<typename EVENT>
	Event::EventDelegate<EVENT>*
		getDelegate();

	// get the delegateID with the EVENT.
	template<typename EVENT>
	Event::DelegateID
		getDelegateID();

	// Register a function to be called when the Listener is destoried.
	void doWhenDeconstruct(std::function<void(void)> task);

private:
	// This function is called when the Listener is destoried 
	// whether on the stack or the heap.
	// Its main goal is to avoid the situation when listener is destoried,
	// but some one still hold its pointer.
	// Every on can add a function to the Listener, which will be called at the deconstruction period.
	std::vector<std::function<void(void)>> m_tasksWhenDeconstruct;
};

template<typename DERIVED_LISTENER, typename ...LISTENED_EVENTS>
template<typename EVENT>
inline Event::EventDelegate<EVENT>* EventListener<DERIVED_LISTENER, LISTENED_EVENTS...>::getDelegate()
{
	// Declare the pointer to the member function.
	void(DERIVED_LISTENER::*callBack)(EVENT *);
	callBack = &DERIVED_LISTENER::ListenEvent;

	// Wrap the member function and the host to a EventDelegateWrapper.
	return new Event::EventDelegateWrapper<DERIVED_LISTENER, EVENT>
		(reinterpret_cast<DERIVED_LISTENER*>(this), callBack);
}

template<typename DERIVED_LISTENER, typename ...LISTENED_EVENTS>
template<typename EVENT>
inline Event::DelegateID EventListener<DERIVED_LISTENER, LISTENED_EVENTS...>::getDelegateID()
{
	// the same delegate of the same listener 
	// will use the same receiver type and event type.
	// only the pointer to reciever is different.
	// so here use a static var to store the typeID,
	// but the pointer to reciever will be set with differ object.
	static Event::DelegateID returnedID = {
		Event::RecvIDGenerator::newID<DERIVED_LISTENER>(),
		0,	// init as nullptr
		Event::EventIDGenerator::newID<EVENT>() };

	// set the pointer to the listener
	returnedID.pReciever = reinterpret_cast<Event::ID>(this);
	return returnedID;
}

template<typename DERIVED_LISTENER, typename ...LISTENED_EVENTS>
inline EventListener<DERIVED_LISTENER, LISTENED_EVENTS...>::~EventListener()
{
	for (auto & doTheJob : m_tasksWhenDeconstruct)
	{
		doTheJob();
	}
}

template<typename DERIVED_LISTENER, typename ...LISTENED_EVENTS>
inline void EventListener<DERIVED_LISTENER, LISTENED_EVENTS...>::doWhenDeconstruct(std::function<void(void)> task)
{
	m_tasksWhenDeconstruct.push_back(task);
}

}// namespace Event

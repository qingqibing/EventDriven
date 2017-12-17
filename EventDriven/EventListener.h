#pragma once
#include "EventDelegate.h"
#include "EventDispatcher.h"

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
};

template<typename DERIVED_LISTENER, typename ...LISTENED_EVENTS>
template<typename EVENT>
inline Event::EventDelegate<EVENT>* EventListener<DERIVED_LISTENER, LISTENED_EVENTS...>::getDelegate()
{
	void(DERIVED_LISTENER::*callBack)(EVENT *);
	callBack = &DERIVED_LISTENER::ListenEvent;
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

}// namespace Event

#pragma once
#include "EventDelegate.h"
#include "EventDispatcher.h"

namespace Event
{
// next two class {IEventListener/EventListener} have
// no significant function,
// the main goal of them is to simplify the process
// add delegate to the EventHandler.

// IEventListener is not a interface,
// it has no virtual function,
// but it functionality is same as the interface,
// it is mainly used in the EventHandler.
// Throught the function{getDelegate()},
// the EventHandler can get the delegate from it.
// And the template arg DERIVED_LISTENER is the 
// final Class which implement the callBack function,
// all the callBack function should have the same name
// "ListenEvent", but can have different parameter type( only 1 parameter)
// for different EventType.
template<typename DERIVED_LISTENER>
class IEventListener
{
public:
	typedef DERIVED_LISTENER DerviedListener;

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

template<typename DERIVED_LISTENER>
template<typename EVENT>
inline Event::EventDelegate<EVENT>* IEventListener<DERIVED_LISTENER>::getDelegate()
{
	void(DerviedListener::*callBack)(EVENT *);
	callBack = &DerviedListener::ListenEvent;
	return new Event::EventDelegateWrapper<DERIVED_LISTENER, EVENT>
		(reinterpret_cast<DERIVED_LISTENER*>(this), callBack);
}

template<typename DERIVED_LISTENER>
template<typename EVENT>
inline Event::DelegateID IEventListener<DERIVED_LISTENER>::getDelegateID()
{
	// the same delegate of the same listener 
	// will use the same receiver type and event type.
	// only the pointer to reciever is different.
	// so here use a static var to store the typeID,
	// but the pointer to reciever will be set with differ object.
	static DelegateID returnedID = {
		Event::RecvIDGenerator::newID<DERIVED_LISTENER>(),
		0,	// init as nullptr
		Event::EventIDGenerator::newID<EVENT>() };

	// set the pointer to the listener
	returnedID.pReciever = reinterpret_cast<Event::ID>(this);
	return returnedID;
}

// The EventListener is barely a template storage place,
// it has no function, all it goal is to store the desired Event Type,
// and pass the class(DERIVED_LISTENER) which implement the callBack
// function forward to the IEventListener. 
template<typename DERIVED_LISTENER, typename...LISTENED_EVENTS>
class EventListener
	:public IEventListener<DERIVED_LISTENER>
{

};


}// namespace Event

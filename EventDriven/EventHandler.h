#pragma once
#include "EventDelegate.h"
#include "EventDispatcher.h"
#include "EventListener.h"

namespace Event
{

class EventHandler
{
public:
	// interface to the EventMarket
	class IEventMarket
	{
	public:
		virtual void dispatch() = 0;
	};

private:
	// the real EventMarket
	template<typename EVENT_TYPE>
	class EventMarket
		:public IEventMarket
	{
	public:
		std::vector<EVENT_TYPE*> bufferedEvents;
		EventDispatcher<EVENT_TYPE> dispatcher;

	
		// Implement the function to dispatch all the events;
		void dispatch() override
		{
			for (auto * e : bufferedEvents)
			{
				dispatcher.dispatch(e);
			}
			bufferedEvents.clear();
		}
	};
public:
	template<typename EVENT_TYPE>
	void addDelegate(EventDelegate<EVENT_TYPE> * dlgate);

	template<typename EVENT_TYPE>
	void removeDelegate(const DelegateID& removedID);

	template<typename EVENT_TYPE>
	void sendEvent(EVENT_TYPE* e);

	// send every event to corresponding dispathcer.
	void dispatchAll();

	template<typename DERIVED_LISTENER, typename ...LISTENED_EVENTS>
	void registerListener(EventListener<DERIVED_LISTENER, LISTENED_EVENTS...> * pListener);

	template<typename DERIVED_LISTENER, typename ...LISTENED_EVENTS>
	void unregisterListener(EventListener<DERIVED_LISTENER, LISTENED_EVENTS...> * pListener);

	

	// recursion
	template<typename DERIVED_LISTENER, typename FIRST_EVENT, typename ...REST_EVENTS>
	void registerListenerRecursion(IEventListener<DERIVED_LISTENER> * pListener);
	// stop recursion ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	template<typename DERIVED_LISTENER>
	void registerListenerRecursion(IEventListener<DERIVED_LISTENER> * pListener);


	// recursion
	template<typename DERIVED_LISTENER, typename FIRST_EVENT, typename ...REST_EVENTS>
	void unregisterListenerRecursion(IEventListener<DERIVED_LISTENER> * pListener);
	// stop recursion ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	template<typename DERIVED_LISTENER>
	void unregisterListenerRecursion(IEventListener<DERIVED_LISTENER> * pListener);


private:
	template<typename EVENT_TYPE>
	static EventMarket<EVENT_TYPE>* getEventMarket();

	// this function should only be called inside the getEventMarket();
	// if functionality is to create a EventMarket,
	// store the market pointer in the vector,
	// then the EventHandler can access these market;
	template<typename EVENT_TYPE>
	static EventMarket<EVENT_TYPE>* newEventMarketAndStore();

private:
	static std::vector<IEventMarket*> markets;
	
};

template<typename EVENT_TYPE>
inline void EventHandler::addDelegate(EventDelegate<EVENT_TYPE>* dlgate)
{
	static EventHandler::EventMarket<EVENT_TYPE>* pEMarket = getEventMarket<EVENT_TYPE>();
	pEMarket->dispatcher.addDelegate(dlgate);
}

template<typename EVENT_TYPE>
inline void EventHandler::removeDelegate(const DelegateID& removedID)
{
	static auto * pMarket = this->getEventMarket<EVENT_TYPE>();
	pMarket->dispatcher.removeDelegate(removedID);
}

template<typename EVENT_TYPE>
inline void EventHandler::sendEvent(EVENT_TYPE * e)
{
	static EventHandler::EventMarket<EVENT_TYPE> * eventMarket = getEventMarket<EVENT_TYPE>();
	eventMarket->bufferedEvents.push_back(e);
}

template<typename DERIVED_LISTENER, typename ...LISTENED_EVENTS>
inline void EventHandler::registerListener(EventListener<DERIVED_LISTENER, LISTENED_EVENTS...>* pListener)
{
	registerListenerRecursion<DERIVED_LISTENER, LISTENED_EVENTS...>
		(reinterpret_cast<IEventListener<DERIVED_LISTENER>*>(pListener));
}

template<typename DERIVED_LISTENER, typename ...LISTENED_EVENTS>
inline void EventHandler::unregisterListener(EventListener<DERIVED_LISTENER, LISTENED_EVENTS...>* pListener)
{
	unregisterListenerRecursion<DERIVED_LISTENER, LISTENED_EVENTS...>
		(reinterpret_cast<IEventListener<DERIVED_LISTENER>*>(pListener));
}

template<typename DERIVED_LISTENER, typename FIRST_EVENT, typename ...REST_EVENTS>
inline void EventHandler::registerListenerRecursion(IEventListener<DERIVED_LISTENER> * pListener)
{
	addDelegate<FIRST_EVENT>(pListener->getDelegate<FIRST_EVENT>());
	registerListenerRecursion<DERIVED_LISTENER, REST_EVENTS...>(pListener);
}

template<typename DERIVED_LISTENER>
inline void EventHandler::registerListenerRecursion(IEventListener<DERIVED_LISTENER>* pListener)
{
	// stop the recursion.
}

template<typename DERIVED_LISTENER, typename FIRST_EVENT, typename ...REST_EVENTS>
inline void EventHandler::unregisterListenerRecursion(IEventListener<DERIVED_LISTENER>* pListener)
{
	removeDelegate(pListener->getDelegateID<FIRST_EVENT>());
	unregisterListenerRecursion<DERIVED_LISTENER, REST_EVENTS...>(pListener);
}

template<typename EVENT_TYPE>
inline EventHandler::EventMarket<EVENT_TYPE>* EventHandler::getEventMarket()
{
	static EventMarket<EVENT_TYPE>* eventMarket = newEventMarketAndStore<EVENT_TYPE>();
	return eventMarket;
}

template<typename EVENT_TYPE>
inline EventHandler::EventMarket<EVENT_TYPE>* EventHandler::newEventMarketAndStore()
{
	// basically, this function should only be called inside the getEventMarket(),
	// because this function will append the market into the markets,
	// which will be controled by the EventHandler.
	// but for simplification, here we still use the static variable
	// to ensure that there is only one copy of the market.
	// But the operation of push it into the std::vector<IEventMarket*> can't
	// ensure this. so be awared.
	static EventHandler::EventMarket<EVENT_TYPE> eventMarket;
	auto * pEventMarket = &eventMarket;
	markets.push_back(pEventMarket);
	return pEventMarket;
}

}// namespace Event


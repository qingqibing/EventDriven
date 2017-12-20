#pragma once
#include "EventDelegate.h"
#include "EventDispatcher.h"
#include "EventListener.h"

namespace Event
{
// interface to the EventMarket
class IEventMarket
{
public:
	virtual void dispatch() = 0;
	// clear and delete all the delegates
	virtual void clearDelegates() = 0;
};

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

	void clearDelegates()
	{
		dispatcher.clearDelegates();
	}
};

template<typename HANDLER_SERIES>
class EventHandler
{
public:
	static void clearDelegates();

	template<typename EVENT_TYPE>
	static void addDelegate(EventDelegate<EVENT_TYPE> * dlgate);

	template<typename EVENT_TYPE>
	static void removeDelegate(const DelegateID& removedID);

	template<typename EVENT_TYPE>
	static void sendEvent(EVENT_TYPE* e);

	// send every event to corresponding dispathcer.
	static void dispatchAll();

	template<typename DERIVED_LISTENER, typename ...LISTENED_EVENTS>
	static void registerListener(EventListener<DERIVED_LISTENER, LISTENED_EVENTS...> * pListener);

	template<typename DERIVED_LISTENER, typename ...LISTENED_EVENTS>
	static void unregisterListener(EventListener<DERIVED_LISTENER, LISTENED_EVENTS...> * pListener);

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

template<typename HANDLER_SERIES>
template<typename EVENT_TYPE>
inline void EventHandler<HANDLER_SERIES>::addDelegate(EventDelegate<EVENT_TYPE>* dlgate)
{
	static EventMarket<EVENT_TYPE>* pEMarket = getEventMarket<EVENT_TYPE>();
	pEMarket->dispatcher.addDelegate(dlgate);
}

template<typename HANDLER_SERIES>
template<typename EVENT_TYPE>
inline void EventHandler<HANDLER_SERIES>::removeDelegate(const DelegateID& removedID)
{
	static auto * pMarket = getEventMarket<EVENT_TYPE>();
	pMarket->dispatcher.removeDelegate(removedID);
}

template<typename HANDLER_SERIES>
template<typename EVENT_TYPE>
inline void EventHandler<HANDLER_SERIES>::sendEvent(EVENT_TYPE * e)
{
	static EventMarket<EVENT_TYPE> * eventMarket = getEventMarket<EVENT_TYPE>();
	eventMarket->bufferedEvents.push_back(e);
}

template<typename HANDLER_SERIES>
template<typename DERIVED_LISTENER, typename ...LISTENED_EVENTS>
inline void EventHandler<HANDLER_SERIES>::registerListener(EventListener<DERIVED_LISTENER, LISTENED_EVENTS...>* pListener)
{
	bool zeros[] = {(addDelegate<LISTENED_EVENTS>(pListener->getDelegate<LISTENED_EVENTS>()), false)...};
	//registerListenerRecursion<DERIVED_LISTENER, LISTENED_EVENTS...>
	//	(reinterpret_cast<IEventListener<DERIVED_LISTENER>*>(pListener));
}

template<typename HANDLER_SERIES>
template<typename DERIVED_LISTENER, typename ...LISTENED_EVENTS>
inline void EventHandler<HANDLER_SERIES>::unregisterListener(EventListener<DERIVED_LISTENER, LISTENED_EVENTS...>* pListener)
{
	bool zeros[] = { (removeDelegate<LISTENED_EVENTS>(pListener->getDelegateID<LISTENED_EVENTS>()), false)... };

	//unregisterListenerRecursion<DERIVED_LISTENER, LISTENED_EVENTS...>
	//	(reinterpret_cast<IEventListener<DERIVED_LISTENER>*>(pListener));
}

template<typename HANDLER_SERIES>
template<typename EVENT_TYPE>
inline EventMarket<EVENT_TYPE>* EventHandler<HANDLER_SERIES>::getEventMarket()
{
	static typename EventMarket<EVENT_TYPE>* eventMarket = newEventMarketAndStore<EVENT_TYPE>();
	return eventMarket;
}

template<typename HANDLER_SERIES>
template<typename EVENT_TYPE>
inline EventMarket<EVENT_TYPE>* EventHandler<HANDLER_SERIES>::newEventMarketAndStore()
{
	// basically, this function should only be called inside the getEventMarket(),
	// because this function will append the market into the markets,
	// which will be controled by the EventHandler.
	// but for simplification, here we still use the static variable
	// to ensure that there is only one copy of the market.
	// But the operation of push it into the std::vector<IEventMarket*> can't
	// ensure this. so be awared.
	static EventMarket<EVENT_TYPE> eventMarket;
	auto * pEventMarket = &eventMarket;
	markets.push_back(pEventMarket);
	return pEventMarket;
}


template<typename HANDLER_SERIES>
// Initialize the static variable.
std::vector<IEventMarket*> EventHandler<HANDLER_SERIES>::markets;

template<typename HANDLER_SERIES>
void EventHandler<HANDLER_SERIES>::clearDelegates()
{
	for (auto * market : markets)
	{
		market->clearDelegates();
	}
}

template<typename HANDLER_SERIES>
void EventHandler<HANDLER_SERIES>::dispatchAll()
{
	for (auto * market : markets)
	{
		market->dispatch();
	}
}

}// namespace Event


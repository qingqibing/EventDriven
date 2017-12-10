#pragma once
#include "IEventDelegate.h"
#include "IEventDispatcher.h"

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
			for (auto & e : bufferedEvents)
			{
				dispatcher.dispatch(e);
			}
		}
	};
public:
	template<typename EVENT_TYPE>
	void addDelegate(EventDelegate<EVENT_TYPE> * dlgate);
	template<typename EVENT_TYPE>
	void sendEvent(EVENT_TYPE* e);

	// send every event to corresponding dispathcer.
	void dispatchAll();


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
inline void EventHandler::sendEvent(EVENT_TYPE * e)
{
	static EventHandler::EventMarket<EVENT_TYPE> * eventMarket = getEventMarket<EVENT_TYPE>();
	eventMarket->bufferedEvents.push_back(e);
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


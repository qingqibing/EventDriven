#pragma once
#include "IEventDelegate.h"
#include <vector>

namespace Event
{
template<typename EVENT_TYPE>
class EventDispatcher
{
	typedef EventDelegate<EVENT_TYPE> InsideDelegate;

public:
	EventDispatcher();
	~EventDispatcher();
	virtual void dispatch(EVENT_TYPE * e);
	virtual void addDelegate(InsideDelegate * newDelegate);

private:
	std::vector<InsideDelegate*> delegateList;
};

template<typename EVENT_TYPE>
inline EventDispatcher<EVENT_TYPE>::EventDispatcher()
{
}

template<typename EVENT_TYPE>
inline EventDispatcher<EVENT_TYPE>::~EventDispatcher()
{
}

template<typename EVENT_TYPE>
inline void EventDispatcher<EVENT_TYPE>::dispatch(EVENT_TYPE * e)
{
	for (auto & dlgate : delegateList)
	{
		dlgate->invoke(e);
	}
}

template<typename EVENT_TYPE>
inline void EventDispatcher<EVENT_TYPE>::addDelegate(InsideDelegate * newDelegate)
{
	delegateList.push_back(newDelegate);
}

}// namespace Event

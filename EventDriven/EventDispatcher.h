#pragma once
#include "EventDelegate.h"
#include <vector>
#include <algorithm>

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
	virtual void removeDelegate(InsideDelegate * removedDeletate);

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
	for (auto * dlgate : delegateList)
	{
		dlgate->invoke(e);
	}
}

template<typename EVENT_TYPE>
inline void EventDispatcher<EVENT_TYPE>::addDelegate(InsideDelegate * newDelegate)
{
	delegateList.push_back(newDelegate);
}

template<typename EVENT_TYPE>
inline void EventDispatcher<EVENT_TYPE>::removeDelegate(InsideDelegate * removedDeletate)
{
	auto removedLocation = std::find_if(delegateList.begin(), delegateList.end(),
		[&removedDeletate](InsideDelegate * cmpDlg) { return *cmpDlg == *removedDeletate; });
	if (removedLocation != delegateList.end())
	{
		InsideDelegate * pMm = reinterpret_cast<InsideDelegate *>(*removedLocation);
		// the delegate is on the heap,
		// so we should free its memory.
		delete pMm;
		delegateList.erase(removedLocation);
	}

	// the removedDelegate should not be delete.
}

}// namespace Event

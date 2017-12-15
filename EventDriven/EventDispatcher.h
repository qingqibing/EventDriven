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
	void dispatch(EVENT_TYPE * e);
	void addDelegate(InsideDelegate * newDelegate);
	void removeDelegate(InsideDelegate * removedDeletate);
	// removed the delegate through the deleteGateID
	void removeDelegate(DelegateID removedID);

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

template<typename EVENT_TYPE>
inline void EventDispatcher<EVENT_TYPE>::removeDelegate(DelegateID removedID)
{
	auto removedLocation = std::find_if(delegateList.begin(), delegateList.end(),
		[&removedID](InsideDelegate * cmpDlg) { return cmpDlg->getID() == removedID; });
	if (removedLocation != delegateList.end())
	{
		InsideDelegate * pMm = reinterpret_cast<InsideDelegate *>(*removedLocation);
		// the delegate is on the heap,
		// so we should free its memory.
		delete pMm;
		delegateList.erase(removedLocation);
	}
}

}// namespace Event

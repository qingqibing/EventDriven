#include "EventHandler.h"

namespace Event
{

// Initialize the static variable.
std::vector<EventHandler::IEventMarket*> EventHandler::markets;

void EventHandler::dispatchAll()
{
	for (auto & market : markets)
	{
		market->dispatch();
	}
}

} // namespace Event

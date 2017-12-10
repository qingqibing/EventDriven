#include <iostream>
#include <stdio.h>
#include <MyTools/UnitTestModules.h>
#include "../EventDriven/TypeTools.h"
#include <TypeTools.h>
#include <IEventDelegate.h>
#include "TryEventDelegate.h"
#include <IEventDispatcher.h>
#include <EventHandler.h>

#pragma comment(lib, "EventDriven")

DECLARE_TEST_UNITS;

namespace TestUnit
{

void GetReady()
{
	std::cout << "ready" << std::endl;
}

void AfterTest()
{
	std::cout << "test end" << std::endl;
}

void AddTestUnit()
{
	TEST_UNIT_START("test TypeIDGenerator")
		int error = 0;
		auto int_0 = TypeTool::IDGenerator<int>::newID<int>();
		auto int_1 = TypeTool::IDGenerator<int>::newID<char>();
		auto int_2 = TypeTool::IDGenerator<int>::newID<std::vector<int>>();


		error += NOT_EQ(int_0, TypeTool::IDGenerator<int>::newID<int>());
		error += NOT_EQ(int_1, TypeTool::IDGenerator<int>::newID<char>());
		error += NOT_EQ(int_2, TypeTool::IDGenerator<int>::newID<std::vector<int>>());
		return error == 0;
	TEST_UNIT_END;

	TEST_UNIT_START("try eventDelegate")
		int error = 0;
		static std::vector<Event::EventDelegate<TestEvent> *> delegations;

		TestListener tListenser;
		tListenser.registerDelegate(delegations);

		TestEvent tEvent;
		tEvent.x = 22;
		tEvent.y = 33;

		for (auto & dlgate : delegations)
		{
			dlgate->invoke(&tEvent);
		}

		error += NOT_EQ(22, tListenser.recvX);
		error += NOT_EQ(33, tListenser.recvY);

		return error == 0;
	TEST_UNIT_END;

	TEST_UNIT_START("test IEventDispatcher")
		int error = 0;
		Event::EventDispatcher<TestEvent> dispatcher;

		TestListener listener1;

		TestEvent tEvent1;

		// init events
		tEvent1.x = 22;
		tEvent1.y = 33;

		dispatcher.addDelegate(
			new Event::EventDelegateWrapper<TestListener, TestEvent>(
				&listener1, &TestListener::waitTestEvent));


		error += EQ(22, listener1.recvX);
		error += EQ(33, listener1.recvY);

		dispatcher.dispatch(&tEvent1);

		error += NOT_EQ(22, listener1.recvX);
		error += NOT_EQ(33, listener1.recvY);

		
		return error == 0;
	TEST_UNIT_END;

	TEST_UNIT_START("test event handler")
		int error = 0;
		Event::EventHandler handler;

		TestListener listener1;
		TestListenerSecond listener2;

		TestEvent tEvent1;
		TestEventSencond tEvent2;

		// init events
		tEvent1.x = 22;
		tEvent1.y = 33;

		tEvent2.r = 44;
		tEvent2.g = 55;
		tEvent2.b = 66;

		// register listener
		handler.addDelegate(
			new Event::EventDelegateWrapper<TestListener, TestEvent>(
				&listener1, &TestListener::waitTestEvent));
		handler.addDelegate(
			new Event::EventDelegateWrapper<TestListenerSecond, TestEventSencond>(
				&listener2, &TestListenerSecond::waitSecondEvent));
		
		handler.sendEvent(&tEvent1);
		handler.sendEvent(&tEvent2);

		error += EQ(22, listener1.recvX);
		error += EQ(33, listener1.recvY);
		error += EQ(44, listener2._r);
		error += EQ(55, listener2._g);
		error += EQ(66, listener2._b);

		handler.dispatchAll();

		error += NOT_EQ(22, listener1.recvX);
		error += NOT_EQ(33, listener1.recvY);
		error += NOT_EQ(44, listener2._r);
		error += NOT_EQ(55, listener2._g);
		error += NOT_EQ(66, listener2._b);

		return error == 0;
	TEST_UNIT_END;
}
}// namespace TestUnit



int main()
{
	TestUnit::testMain();

	getchar();

	return 0;
}


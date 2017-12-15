#include <iostream>
#include <stdio.h>
#include <MyTools/UnitTestModules.h>
#include <TypeTool.h>
#include <EventDelegate.h>
#include "TryEventDelegate.h"
#include <EventDispatcher.h>
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
	// test TypeIDGenerator
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
	}

	// try eventDelegate
	{
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
	}

	// test IEventDispatcher
	{
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
	}

	// test IEventDispatcher remove delegate
	{
	TEST_UNIT_START("test IEventDispatcher remove delegate")
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


		auto * removedDelegate = new Event::EventDelegateWrapper<TestListener, TestEvent>(
			&listener1, &TestListener::waitTestEvent);
		// then we remove the delegate
		dispatcher.removeDelegate(removedDelegate);

		// then we reset the listener's data to see if it can receive the event,
		// correctly its data should not change after the diapatch.

		listener1.recvX = 0;
		listener1.recvY = 0;

		// ensure the event data is sending the desired data,
		// and the listener1 should not receive the data.
		error += NOT_EQ(22, tEvent1.x);
		error += NOT_EQ(33, tEvent1.y);

		dispatcher.dispatch(&tEvent1);
		
		error += NOT_EQ(0, listener1.recvX);
		error += NOT_EQ(0, listener1.recvY);

		// here we add it again and the dispatcher should send the data to the listener.
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
	}

	// test IEventDispatcher remove delegate with delegateID
	{
		TEST_UNIT_START("test IEventDispatcher remove delegate with delegateID")
			int error = 0;
			Event::EventDispatcher<TestEvent> dispatcher;

			TestListener listener1;

			TestEvent tEvent1;

			// init events
			tEvent1.x = 22;
			tEvent1.y = 33;

			auto testDelegate = new Event::EventDelegateWrapper<TestListener, TestEvent>(
				&listener1, &TestListener::waitTestEvent);

			dispatcher.addDelegate(testDelegate);

			error += EQ(22, listener1.recvX);
			error += EQ(33, listener1.recvY);

			dispatcher.dispatch(&tEvent1);

			error += NOT_EQ(22, listener1.recvX);
			error += NOT_EQ(33, listener1.recvY);

			dispatcher.removeDelegate(testDelegate->getID());

			// then we reset the listener's data to see if it can receive the event,
			// correctly its data should not change after the diapatch.

			listener1.recvX = 0;
			listener1.recvY = 0;

			// ensure the event data is sending the desired data,
			// and the listener1 should not receive the data.
			error += NOT_EQ(22, tEvent1.x);
			error += NOT_EQ(33, tEvent1.y);

			dispatcher.dispatch(&tEvent1);

			error += NOT_EQ(0, listener1.recvX);
			error += NOT_EQ(0, listener1.recvY);

			// here we add it again and the dispatcher should send the data to the listener.
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
	}

	// test event handler
	{
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

	// test delete delegate from event handler
	{
		TEST_UNIT_START("test delete delegate from event handler")
			int invalidEventAData	= 1;
			int invalidEventBData	= 2;
			// |||||||||||||||||||||||||
			int validEventAData		= 66;
			int validEventBData		= 55;

			Event::EventHandler handler;

			ListnerA listenerA_1;

			EventA eventA_1;
			EventB eventB_1;
			eventA_1.data = validEventAData;
			eventB_1.data = validEventBData;

			// a lambda to reset the listener.
			auto resetListener = [&]() {
				// reset the listener
				listenerA_1.eventAData = invalidEventAData;
				listenerA_1.eventBData = invalidEventBData;
			};

			// create a lambda to send event and dispatch them.
			auto runEventHandler = [&]() {
				handler.sendEvent(&eventA_1);
				handler.sendEvent(&eventB_1);

				// call delegate
				handler.dispatchAll();
			};

			auto * delegateA_1 = listenerA_1.getDelegateA();
			auto * delegateB_1 = listenerA_1.getDelegateB();

			// store the delegate id for later usage.
			Event::DelegateID dIDA_1 = delegateA_1->getID();
			Event::DelegateID dIDB_1 = delegateB_1->getID();
			handler.addDelegate(delegateA_1);
			handler.addDelegate(delegateB_1);

			runEventHandler();

			// ensure the data is sended
			errorLogger += NOT_EQ(validEventAData, listenerA_1.eventAData);
			errorLogger += NOT_EQ(validEventBData, listenerA_1.eventBData);

			// phase one remove delegate of EventA
			// and test the listener only receive EventB
			{
			// just delete the delegate of EventA from the handler
			handler.removeDelegate<EventA>(dIDA_1);

			resetListener();
			runEventHandler();

			// ensure only EventB is sended
			errorLogger += NOT_EQ( invalidEventAData , listenerA_1.eventAData);
			errorLogger += NOT_EQ(validEventBData, listenerA_1.eventBData);
			}

			// phase two remove delegate of EventB
			// re add the delegate of EventA
			// and test the listener only receive EventA
			{
			// the orignal delegateA_1 in the heap should have been delete
			// in the phase one, (by the dispatcher in the handler).
			delegateA_1 = listenerA_1.getDelegateA();
			handler.addDelegate(delegateA_1);
			handler.removeDelegate<EventB>(dIDB_1);

			resetListener();
			runEventHandler();

			// ensure only EventA is sended
			errorLogger += NOT_EQ(validEventAData, listenerA_1.eventAData);
			errorLogger += NOT_EQ( invalidEventBData , listenerA_1.eventBData);
			}

			// phase three remove delegate of EventA
			// now all delegate of the listener have been removed,
			// it shouldn't receive any event
			{
			handler.removeDelegate<EventA>(dIDA_1);

			resetListener();
			runEventHandler();

			// ensure listener DON'T receive any data.
			errorLogger += NOT_EQ( invalidEventAData, listenerA_1.eventAData);
			errorLogger += NOT_EQ( invalidEventBData, listenerA_1.eventBData);
			}

			return errorLogger.conclusion();
		TEST_UNIT_END;
	}

	// test delegate id
	{
	TEST_UNIT_START("test delegate id")
		int error = 0;
		ListnerA lisA;
		Event::EventDelegate<EventA> * delegateAA = lisA.getDelegateA();
		Event::EventDelegate<EventB> * delegateAB = lisA.getDelegateB();

		ListnerB lisB;
		auto * delegateBA = lisB.getDelegateA();
		auto * delegateBB = lisB.getDelegateB();

		error += (*delegateAA) == (*delegateBA);
		error += (*delegateAB) == (*delegateBB);
		return error == 0;
	TEST_UNIT_END;
	}

	// test event listener
	{
	TEST_UNIT_START("test event listener")
		int error = 0;
		Event::EventHandler eHandler;
		
		AdvanceListener advListener;
		SecondAdvanceListener secondAListener;

		eHandler.registerListener(&advListener);
		eHandler.registerListener(&secondAListener);

		EventA testEventA;
		EventB testEventB;

		EventC testEventC_1;
		EventD testEventD_1;
		EventE testEventE_1;
		//||||||||||||||||||
		EventD testEventD_2;
		EventE testEventE_2;

		testEventA.data = 5;
		testEventB.data = 8;
		testEventC_1.data = 1;
		testEventD_1.data = 2;
		testEventE_1.data = 3;
		//|||||||||||||||||||
		testEventD_2.data = 4;
		testEventE_2.data = 5;

		eHandler.sendEvent(&testEventA);
		eHandler.sendEvent(&testEventB);
		eHandler.sendEvent(&testEventC_1);
		eHandler.sendEvent(&testEventD_1);
		eHandler.sendEvent(&testEventE_1);
		eHandler.sendEvent(&testEventD_2);
		eHandler.sendEvent(&testEventE_2);


		error += EQ(5, advListener.eventAData);
		error += EQ(8, advListener.eventBData);
		// ensure the secondAListener's initial event data is all 0¡£
		error += NOT_EQ(0, secondAListener.eventCData);
		error += NOT_EQ(0, secondAListener.eventDData);
		error += NOT_EQ(0, secondAListener.eventEData);

		eHandler.dispatchAll();

		error += NOT_EQ(5, advListener.eventAData);
		error += NOT_EQ(8, advListener.eventBData);
		// 1
		error += NOT_EQ(1, secondAListener.eventCData);
		// 2 + 4 = 6
		error += NOT_EQ(6, secondAListener.eventDData);
		// 3 + 5 = 8
		error += NOT_EQ(8, secondAListener.eventEData);

		return error == 0;
	TEST_UNIT_END;
	}
}
}// namespace TestUnit



int main()
{
	TestUnit::testMain();

	getchar();

	return 0;
}


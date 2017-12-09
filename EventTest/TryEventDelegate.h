#pragma once

#include <TypeTools.h>
#include <IEvent.h>
#include <IEventDelegate.h>
#include <iostream>
#include <vector>

class TestEvent
	:public Event::IEvent
{
public:
	TestEvent() : x(1), y(2) {}
	int x, y;
};

class TestListener
{
	
	void waitTestEvent(TestEvent * e)
	{
		std::cout 
			<< "TestListener have receive the TestEvent" << std::endl
			<< "event.x = " << e->x << std::endl
			<< "event.y = " << e->y << std::endl;
		recvX = e->x;
		recvY = e->y;
	}

public:
	void registerDelegate(std::vector<Event::IEventDelegate *>& queue)
	{
		queue.push_back(new Event::EventDelegate<TestListener, TestEvent>(this, &TestListener::waitTestEvent));
	}

	int recvX, recvY;
};
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

struct TestEventSencond
{
public:
	int r, g, b;
};

class TestListener
{
public:
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
	void registerDelegate(std::vector<Event::EventDelegate<TestEvent> *>& queue)
	{
		queue.push_back(
			new Event::EventDelegateWrapper<TestListener, TestEvent>(this, &TestListener::waitTestEvent));
	}

	int recvX, recvY;
};

struct TestListenerSecond
{
public:

	void waitSecondEvent(TestEventSencond * e)
	{
		_r = e->r;
		_g = e->g;
		_b = e->b;
	}

	int _r, _g, _b;
};

inline void someFunc()
{
	static int a = 0;
}
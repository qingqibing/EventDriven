#pragma once

#include <TypeTool.h>
#include <EventDelegate.h>
#include <iostream>
#include <vector>
#include <EventListener.h>

class TestEvent
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

struct EventA
{
	int data;
};
struct EventB
{
	int data;
};

struct EventC
{
	int data;
};

struct EventD
{
	int data;
};

struct EventE
{
	int data;
};

struct ListnerA
{
public:
	bool getEventA;
	int eventAData = 0;

	bool getEventB;
	int eventBData = 0;

	void waitEventA(EventA * e)
	{
		eventAData = e->data;
	}

	void waitEventB(EventB * e)
	{
		eventBData = e->data;
	}

	auto getDelegateA()
	{
		return new Event::EventDelegateWrapper<ListnerA, EventA>(this, &ListnerA::waitEventA);
	}

	auto getDelegateB()
	{
		return new Event::EventDelegateWrapper<ListnerA, EventB>(this, &ListnerA::waitEventB);
	}
};


struct ListnerB
{
public:
	bool getEventA;
	int eventAData = 0;

	bool getEventB;
	int eventBData = 0;

	void waitEventA(EventA * e)
	{
		eventAData = e->data;
	}

	void waitEventB(EventB * e)
	{
		eventBData = e->data;
	}

	auto getDelegateA()
	{
		return new Event::EventDelegateWrapper<ListnerB, EventA>(this, &ListnerB::waitEventA);
	}

	auto getDelegateB()
	{
		return new Event::EventDelegateWrapper<ListnerB, EventB>(this, &ListnerB::waitEventB);
	}
};


class AdvanceListener
	:public Event::EventListener<AdvanceListener, EventA, EventB>
{
public:
	void ListenEvent(EventA * e)
	{
		printf("TestListener get EventA\n");
		eventAData = e->data;
	}

	void ListenEvent(EventB * e)
	{
		printf("TestListener get EventB\n");
		eventBData = e->data;
	}

public:
	int eventAData = 0;
	int eventBData = 0;
};

class SecondAdvanceListener
	:public Event::EventListener<SecondAdvanceListener, EventC, EventD, EventE>
{
public:
	void ListenEvent(EventC * e)
	{
		printf("SAL recieve a EventC\n");
		eventCData += e->data;
	}

	void ListenEvent(EventD * e)
	{
		printf("SAL recieve a EventD\n");
		eventDData += e->data;
	}

	void ListenEvent(EventE * e)
	{
		printf("SAL recieve a EventE\n");
		eventEData += e->data;
	}

public:
	int eventCData = 0;
	int eventDData = 0;
	int eventEData = 0;
};

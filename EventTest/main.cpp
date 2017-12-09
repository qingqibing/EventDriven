#include <iostream>
#include <stdio.h>
#include <MyTools/UnitTestModules.h>
#include "../EventDriven/TypeTools.h"
#include <TypeTools.h>
#include <IEvent.h>
#include <IEventDelegate.h>
#include "TryEventDelegate.h"


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
		static std::vector<Event::IEventDelegate *> delegations;

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
}// namespace TestUnit



int main()
{
	TestUnit::testMain();

	getchar();

	return 0;
}


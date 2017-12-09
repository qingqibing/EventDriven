#include <iostream>
#include <stdio.h>
#include <MyTools/UnitTestModules.h>
#include "../EventDriven/TypeIDGenerator.h"


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
		auto int_0 = TypeIDGenerator<int>::newID<int>();
		auto int_1 = TypeIDGenerator<int>::newID<char>();
		auto int_2 = TypeIDGenerator<int>::newID<std::vector<int>>();


		error += NOT_EQ(int_0, TypeIDGenerator<int>::newID<int>());
		error += NOT_EQ(int_1, TypeIDGenerator<int>::newID<char>());
		error += NOT_EQ(int_2, TypeIDGenerator<int>::newID<std::vector<int>>());
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


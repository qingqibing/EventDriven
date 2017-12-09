#pragma once


namespace TypeTool
{

// this struct is used to generate the id for specific type,
// the id is unique for specific ParentType,
// you can change the ParentType to get other series id.
template<typename Series>
struct IDGenerator
{
public:
	static size_t distribID;
	
	template<typename Type>
	inline static size_t newID()
	{
		static size_t nextID = distribID++;
		return nextID;
	}
};


// initialize the series id of the specific Series type
template<typename Series>
size_t IDGenerator<Series>::distribID = 0;


} // namespace TypeIDTools

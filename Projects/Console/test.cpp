#include "test.h"

Test::Test()
{
	x = 213;
	std::wcout << "Created" << std::endl;
}
std::shared_ptr<Test> Test::Create()
{
	return std::make_shared<Test>();
}
Test::~Test()
{
	std::wcout << "Deleted" << std::endl;
}

void Test::Print()
{
	std::wcout << x << std::endl;
}

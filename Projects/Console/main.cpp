#include <memory>
#include <iostream>

class Test
{
	friend std::_Ref_count_obj<Test>;

	int x;

private:
	Test() 
	{
		x = 213;
		std::wcout << "Created" << std::endl;
	}
	Test(Test&) = delete;

public:
	static std::shared_ptr<Test> Create() 
	{
		return std::make_shared<Test>();
	}
	~Test()
	{
		std::wcout << "Deleted" << std::endl;
	}

	void Print()
	{
		std::wcout << x << std::endl;
	}
};

void f()
{
	std::shared_ptr<Test> t = Test::Create();
	t->Print();
}

int wmain()
{
	f();
	system("pause");

	return 0;
}
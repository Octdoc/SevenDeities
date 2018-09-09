#pragma once
#include <memory>
#include <iostream>

#define CLASS(T) class T{public:using P = std::shared_ptr<T>;using W = std::weak_ptr<T>;private:friend std::_Ref_count_obj<T>;
#define END_CLASS };

CLASS(Test)
int x;

private:
	Test();
	Test(Test&) = delete;

public:

	static std::shared_ptr<Test> Create();
	~Test();

	void Print();
END_CLASS
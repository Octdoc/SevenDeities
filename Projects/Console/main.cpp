#include "test.h"

void f()
{
	Test::P t = Test::Create();
	t->Print();
}

int wmain()
{
	f();
	system("pause");

	return 0;
}
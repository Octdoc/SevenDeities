#include "test.h"
#include "genmath.h"
#include <ctime>
#include <chrono>
#include <thread>
#include <future>
#include <mutex>
#include <functional>
#include <forward_list>
#include <Windows.h>


std::mutex m;

template<math::uint N>
void f1()
{
	srand(time(NULL));
	math::Matrix<double, N, N> m1;
	for (math::uint x = 0; x < N; x++)
		for (math::uint y = 0; y < N; y++)
			m1(x, y) = (((double)rand() / RAND_MAX) - 0.5)*10.0;

	auto start = std::chrono::steady_clock::now();
	math::Matrix<double, N, N> m2 = m1.Inverse();
	auto end = std::chrono::steady_clock::now();

	math::Matrix<double, N, N> m3 = m1 * m2;

	m.lock();
	std::cout << std::endl << N << ": Process took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds.";
	std::cout << std::endl << N << ": Process took " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds.";
	std::cout << std::endl << N << ": Process took " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " nanoseconds." << std::endl;

	//m1.Print();
	//m2.Print();
	m3.Print();
	m.unlock();
}


class A
{
protected:
	char a = 'a';
public:
	virtual void PrintAChar()
	{
		std::cout << "PrintAChar - " << 'A' << std::endl;
	}
	void PrintAVar()
	{
		std::cout << "PrintAVar - " << a << std::endl;
	}
};
class B
{
protected:
	char b = 'b';
public:
	virtual void PrintBChar()
	{
		std::cout << "PrintBChar - " << 'B' << std::endl;
	}
	void PrintBVar()
	{
		std::cout << "PrintBVar - " << b << std::endl;
	}
};
class C :public A, public B
{
protected:
	char c = 'c';
public:
	virtual void PrintCChar()
	{
		std::cout << "PrintCChar - " << 'C' << std::endl;
	}
	void PrintCVar()
	{
		std::cout << "PrintCVar - " << c << std::endl;
	}
};

int wmain()
{
	A* c = (A*)new C;

	std::cout << "c->PrintAChar(): ";
	c->PrintAChar();
	std::cout << "c->PrintAVar(): ";
	c->PrintAVar();

	std::cout << "((B*)c)->PrintBChar(): ";
	((B*)c)->PrintBChar();
	std::cout << "((B*)c)->PrintBVar(): ";
	((B*)c)->PrintBVar();

	std::cout << "((B*)(C*)c)->PrintBChar(): ";
	((B*)(C*)c)->PrintBChar();
	std::cout << "((B*)(C*)c)->PrintBVar((): ";
	((B*)(C*)c)->PrintBVar();

	std::cout << "((C*)c)->PrintAChar(): ";
	((C*)c)->PrintAChar();
	std::cout << "((C*)c)->PrintAVar((): ";
	((C*)c)->PrintAVar();

	std::cout << "((C*)c)->PrintBChar(): ";
	((C*)c)->PrintBChar();
	std::cout << "((C*)c)->PrintBVar((): ";
	((C*)c)->PrintBVar();

	std::cout << std::endl;
	system("pause");
	delete c;
	return 0;
}
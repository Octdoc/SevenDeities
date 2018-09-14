#include "test.h"
#include "genmath.h"
#include <ctime>
#include <chrono>


#define SIZE 4
void f1()
{
	srand(time(NULL));
	math::Matrix<double, SIZE, SIZE> m1;
	for (math::uint x = 0; x < SIZE; x++)
		for (math::uint y = 0; y < SIZE; y++)
			m1(x, y) = (((double)rand() / RAND_MAX) - 0.5)*10.0;

	//std::cout << m.Determinant() << std::endl;
	math::Matrix<double, SIZE, SIZE> m2 = m1.Inverse();
	math::Matrix<double, SIZE, SIZE> m3 = m1 * m2;

	m1.Print();
	m2.Print();
	m3.Print();
}

void f2()
{
	double d1[] = { 2,3,4,1,3,1,3,2,2 };
	double d2[] = { 1,2,3 };
	math::Matrix<double, 3, 3> m(d1);
	math::Vector<double, 3> v(d2);

	m.Print();
	v.Print();
	(v*m).Print();
	(m.Trasposed()*v).Print();

	std::cout << sizeof(m) / sizeof(double) << std::endl << sizeof(v) / sizeof(double) << std::endl;
}

int wmain()
{
	auto start = std::chrono::steady_clock::now();

	f1();

	auto end = std::chrono::steady_clock::now();
	std::cout << std::endl << "Process took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds.";
	std::cout << std::endl << "Process took " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds.";
	std::cout << std::endl << "Process took " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " nanoseconds." << std::endl;
	system("pause");
	return 0;
}
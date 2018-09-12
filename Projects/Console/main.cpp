#include "test.h"
#include "genmath.h"
#include <ctime>


void f()
{

}

#define SIZE 7

int wmain()
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

	system("pause");

	return 0;
}
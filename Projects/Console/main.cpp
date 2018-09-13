#include "test.h"
#include "genmath.h"
#include <ctime>


#define SIZE 7
void f()
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


int wmain()
{
	double d1[] = { 2,3,4,1,3,1,3,2,2 };
	double d2[] = { 1,2,3 };
	math::Matrix<double, 3, 3> m(d1);
	math::Vector<double, 3> v(d2);

	m.Print();
	v.Print();
	(v*m).Print();
	(m.Trasposed()*v).Print();

	system("pause");
	return 0;
}
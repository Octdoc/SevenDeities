#include "helpers/hcs_helpers.h"
#include <iostream>

int wmain()
{

	int i[8] = {};
	int& s = i[0];
	int& e = i[1];
	int& n = i[2];
	int& d = i[3];
	int& m = i[4];
	int& o = i[5];
	int& r = i[6];
	int& y = i[7];

	for (int s = 0; s < 10; s++)
	{
		for (int e = 0; e < 10; e++)
		{
			if (e == s)
				continue;
			for (int n = 0; n < 10; n++)
			{
				if (n == s || n == e)
					continue;
				for (int d = 0; d < 10; d++)
				{
					if (d == s || d == e || d == n)
						continue;
					for (int m = 1; m < 10; m++)
					{
						if (m == s || m == e || m == n || m == d)
							continue;
						for (int o = 0; o < 10; o++)
						{
							if (o == s || o == e || o == n || o == d || o == m)
								continue;
							for (int r = 0; r < 10; r++)
							{
								if (r == s || r == e || r == n || r == d || r == m ||r == o)
									continue;
								for (int y = 0; y < 10; y++)
								{
									if (y == s || y == e || y == n || y == d || y == m || y == o || y == r)
										continue;

									int n1 = ((s * 10 + e) * 10 + n) * 10 + d;
									int n2 = ((m * 10 + o) * 10 + r) * 10 + e;
									int sum = (((m * 10 + o) * 10 + n) * 10 + e) * 10 + y;

									if (n1 + n2 == sum)
									{
										std::wcout << ' ' << n1 << std::endl <<
											'+' << n2 << std::endl << sum << std::endl << std::endl;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	system("pause");

	return 0;
}
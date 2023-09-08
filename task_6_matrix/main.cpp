// task_6_matrix.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <map>
#include <vector>
#include <array>
#include <assert.h>
#include <string>

#include "matrix.h"

int main()
{    
	
	matrix<int, 2, 0> mt0;

	for (int i = 0; i < 10; i++)
	{
		mt0[i][i] = i;
		mt0[i][9-i] = 9-i;
	}

	for (int i = 1; i < 9; i++)
	{
		for (int j = 1; j < 9; j++)
		{
			int v = mt0[i][j];
			std::cout << v << " ";
		}
		std::cout << std::endl;
	}
	
	std::cout << "size " << mt0.size() << std::endl;

	for (auto c : mt0)
	{
		int x;
		int y;
		int v;

		std::tie(x, y, v) = c;
		std::cout << x << " " << y << " " << v << std::endl;
	}
	
	return 0;
}

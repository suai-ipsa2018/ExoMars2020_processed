#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

int main(int argc, char**argv)
{
#pragma omp parallel
	{
		int a = 9;
		printf("Hello World! a = %i\n", a);
		//cout << "Hello World! " << "a = " << 9 << " \n" << endl;
	}
	return 0;
}
#include <stdio.h>
#include <iostream>
//#include <GL\glew.h>
#include "Utilities\ClMath.h"
#include "Utilities\ClEngineTypes.h"

int main()
{	
	Vector3f test(0, 1, 0);
	Vector3f test2(1, 0, 0);

	Vector3f test3 = test + test2;

	printf("X:  %f ", test3.GetX());
	printf("Y:  %f ", test3.GetY());
	printf("Z:  %f ", test3.GetZ());


	system("PAUSE");

	return 0;
}
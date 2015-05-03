#include <stdio.h>
#include <iostream>
//#include <GL\glew.h>
#include "Utilities\ClMath.h"
#include "Utilities\ClEngineTypes.h"

int main()
{	
	Quaternion test(1, 0, 0, 1);
	Quaternion test2(0, 1, 0, 1);
	

	Quaternion test3 = test + test2;

	printf("X:  %f ", test3.GetX());
	printf("Y:  %f ", test3.GetY());
	printf("Z:  %f ", test3.GetZ());
	printf("W:  %f ", test3.GetW());


	system("PAUSE");

	return 0;
}
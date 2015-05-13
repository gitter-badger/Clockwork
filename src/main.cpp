#include <stdio.h>
#include <iostream>

#include "Core/Clockwork.h"

int main()
{
	Clockwork engine;
	engine.Init(engine.OpenGL4);
	engine.Run();
	engine.Shutdown();

	return 0;
}

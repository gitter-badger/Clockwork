#include <stdio.h>
#include <iostream>

#include "Core\Clockwork.h"

int main()
{	
	Clockwork engine;
	engine.Init(engine.DirectX11);
	engine.Run();
	engine.Shutdown();

	system("PAUSE");

	return 0;
}
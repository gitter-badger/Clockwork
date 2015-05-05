#pragma once 

#include "../Core/IRenderer.h"

class DXRenderer : public IRenderer
{
public:
	DXRenderer(){/* Not used */ }
	~DXRenderer(){/* Not used */ }

	// Initilaize the renderer
	void Init();

	// Render to the render window
	//TODO: Pass root entity
	void Render();

	//Shutdown the renderer
	void Shutdown();

private:

};


#pragma once 

#include "../Core/IRenderer.h"

class GLRenderer : public IRenderer
{
public:
	GLRenderer(){/* Not used */ }
	~GLRenderer(){/* Not used */ }

	// Initilaize the renderer
	void Init();

	// Render to the render window
	//TODO: Pass root entity
	void Render();
	
	//Shutdown the renderer
	void Shutdown();

private:

};

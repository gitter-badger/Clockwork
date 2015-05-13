#pragma once

#include "../Core/IRenderer.h"
#include "GLWindow.h"

class GLRenderer : public IRenderer
{
public:
	GLRenderer(){/* Not used */ }
	~GLRenderer(){/* Not used */ }

	// Initilaize the renderer
	void Init(int width, int height);

	void Update();

	// Render to the render window
	//TODO: Pass root entity
	void Render();

	//Shutdown the renderer
	void Shutdown();
};

#pragma once

#include "../Core/IRenderer.h"
#include "GL4Window.h"

class GL4Renderer : public IRenderer
{
public:
	GL4Renderer(){/* Not used */ }
	~GL4Renderer(){/* Not used */ }

	// Initilaize the renderer
	void Init(int width, int height);

	void Update();

	// Render to the render window
	//TODO: Pass root entity
	void Render();

	//Shutdown the renderer
	void Shutdown();

private:
    GL4Window m_window;
};

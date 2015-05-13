#include "Clockwork.h"
#include <stdio.h>

void Clockwork::Init(Renderer renderer)
{
	switch (renderer)
	{
	case OpenGL4:
		m_renderer = new GLRenderer;
		break;
	case DirectX11:
#if PLATFORM == PLATFORM_WIN32
		m_renderer = new DXRenderer;
#else
		printf("ERROR: DXRenderer: OS dose not support DX11 \n");
#endif
		break;
	default:
		break;
	}

	m_renderer->Init(1280, 720);
}

void Clockwork::Run()
{
    while(!m_renderer->ShouldClose())
    {
        m_renderer->Update();
        m_renderer->Render();
    }
}

void Clockwork::Shutdown()
{
	m_renderer->Shutdown();
	delete m_renderer;
}

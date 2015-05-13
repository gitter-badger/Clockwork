#include "GLRenderer.h"
#include <stdio.h>

void GLRenderer::Init(int width, int height)
{
    m_window = new GLWindow;
    m_window->CreateWindow(width, height);

	printf("LOG: GLRenderer: Init \n");
}

void GLRenderer::Update()
{
    m_window->UpdateWindow();
    printf("LOG: GLRenderer: Update \n");
}

void GLRenderer::Render()
{
    m_window->SwapBuffers();
	printf("LOG: GLRenderer: Render \n");
}

void GLRenderer::Shutdown()
{
    m_window->Destroy();
    delete m_window;
	printf("LOG: GLRenderer: Shutdown \n");
}

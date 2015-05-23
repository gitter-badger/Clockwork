#include "GL4Renderer.h"
#include <stdio.h>

void GL4Renderer::Init(int width, int height)
{
    m_window.CreateWindow(width, height);
	printf("LOG: GL4Renderer: Init \n");
}

void GL4Renderer::Update()
{
    m_window.UpdateWindow();
    printf("LOG: GL4Renderer: Update \n");
}

void GL4Renderer::Render()
{
    m_window.SwapBuffers();
    m_isClosed = m_window.GetClosed();
	printf("LOG: GL4Renderer: Render \n");
}

void GL4Renderer::Shutdown()
{
    m_window.DestroyWindow();
	printf("LOG: GL4Renderer: Shutdown \n");
}

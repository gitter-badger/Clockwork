#include "GL3Renderer.h"
#include <stdio.h>

void GLRenderer::Init(int width, int height)
{
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	printf("LOG: GLRenderer: Init \n");
}

void GLRenderer::Render()
{
	printf("LOG: GLRenderer: Render \n");
}

void GLRenderer::Shutdown()
{
	printf("LOG: GLRenderer: Shutdown \n");
}

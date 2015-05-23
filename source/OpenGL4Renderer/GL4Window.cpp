#include "GL4Window.h"

void GL4Window::CreateWindow(int width, int height)
{
  m_width = width;
  m_height = height;
  m_isClosed = false;

  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
  {
    fprintf(stderr, "\nUnable to initialize SDL:  %s\n", SDL_GetError());
  }

}

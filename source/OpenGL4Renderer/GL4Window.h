#pragma once

#include "../Core/IWindow.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>

class GL4Window : public IWindow
{
public:
void CreateWindow(int width, int height);
  void UpdateWindow();
  void SwapBuffers();
  void DestroyWindow();
private:
    SDL_Window*     m_window;
    SDL_GLContext   m_context;
};

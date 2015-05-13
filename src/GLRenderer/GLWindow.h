#pragma once

#include "../Core/IWindow.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>

class GLWindow : public IWindow
{
public:

    /*
    *   @brief Creates an OpenGL4 window
    *   @param width is the width of the window
    *   @param height is the height of the window
    */
    void CreateWindow(int width, int height);

    /*
    *   @brief Update the window to clear the screen and other pre-render functions
    */
    void UpdateWindow();

    /*
    *   @brief Swap the back buffer to the front
    */
    void SwapBuffers();

    /*
    *   @brief Destroy the window
    */
    void Destroy();
private:


    SDL_Window* m_window; /**< SDL window that is visable by the user */
    SDL_GLContext m_glContext; /**< A context handling all opengl functionality */
};

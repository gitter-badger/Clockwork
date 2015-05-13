#pragma once

class IWindow
{
public:
    IWindow(){}
    ~IWindow(){}

    virtual void CreateWindow(int width, int height) = 0;
    virtual void UpdateWindow()                      = 0;
    virtual void SwapBuffers()                       = 0;
    virtual void Destroy()                           = 0;

    bool ShouldClose(){ return m_shouldClosed;}
    int GetWidth()    { return m_width;}
    int GetHeight()   { return m_height;}
protected:
    int m_width;
    int m_height;
    bool m_shouldClosed;
};

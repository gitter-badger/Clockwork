#pragma once

#include <stdio.h>

class IWindow
{
public:
  virtual void CreateWindow(int width, int height) = 0;
  virtual void UpdateWindow()                     = 0;
  virtual void SwapBuffers()                      = 0;
  virtual void DestroyWindow()                    = 0;

  int  GetWidth()   { return m_width;   }
  int  GetHeight()  { return m_height;  }
  bool GetClosed()  { return m_isClosed;}

protected:
  int   m_width;
  int   m_height;
  bool  m_isClosed;
};

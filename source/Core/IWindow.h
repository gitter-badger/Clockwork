#pragma once


class IWindow
{
public:
  virtual void CreateWindow(int width, int heigt) = 0;
  virtual void UpdateWindow()                     = 0;
  virtual void SwapBuffers()                      = 0;
  virtual void DestroyWindow()                    = 0;



private:
  int   m_width;
  int   m_height;
  bool  m_isClosed;
};

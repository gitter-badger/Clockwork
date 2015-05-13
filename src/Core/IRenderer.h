#pragma once
#include "IWindow.h"

class IRenderer
{
public:

	IRenderer(){}
	~IRenderer(){}

	virtual void Init(int width, int height) = 0;
	virtual void Update()                    = 0;
	virtual void Render()	                 = 0;
	virtual void Shutdown()                  = 0;

	bool ShouldClose() { return m_window->ShouldClose();}
protected:
    IWindow* m_window;
};

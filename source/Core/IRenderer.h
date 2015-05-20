#pragma once

class IRenderer
{
public:

	IRenderer(){}
	~IRenderer(){}

	virtual void Init(int width, int height) = 0;
	virtual void Update()										 = 0;
	virtual void Render()	                 	 = 0;
	virtual void Shutdown()                  = 0;
};

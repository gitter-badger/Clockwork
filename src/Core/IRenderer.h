#pragma once

class IRenderer
{
public:

	IRenderer(){}
	~IRenderer(){}

	virtual void Init(int width, int height) = 0;
	void Update();
	virtual void Render()	                 = 0;
	virtual void Shutdown()                  = 0;
};

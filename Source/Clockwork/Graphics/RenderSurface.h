

#pragma once

#if defined(CLOCKWORK_OPENGL)
#include "OpenGL/OGLRenderSurface.h"
#elif defined(CLOCKWORK_D3D11)
#include "Direct3D11/D3D11RenderSurface.h"
#else
#include "Direct3D9/D3D9RenderSurface.h"
#endif

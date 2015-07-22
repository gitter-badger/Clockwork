

#pragma once

#if defined(CLOCKWORK_OPENGL)
#include "OpenGL/OGLIndexBuffer.h"
#elif defined(CLOCKWORK_D3D11)
#include "Direct3D11/D3D11IndexBuffer.h"
#else
#include "Direct3D9/D3D9IndexBuffer.h"
#endif

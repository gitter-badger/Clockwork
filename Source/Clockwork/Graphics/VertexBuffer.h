#pragma once

#if defined(CLOCKWORK_OPENGL)
#include "OpenGL/OGLVertexBuffer.h"
#elif defined(CLOCKWORK_D3D11)
#include "Direct3D11/D3D11VertexBuffer.h"
#else
#include "Direct3D9/D3D9VertexBuffer.h"
#endif
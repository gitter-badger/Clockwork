

#pragma once

#if defined(CLOCKWORK_OPENGL)
#include "OpenGL/OGLConstantBuffer.h"
#elif defined(CLOCKWORK_D3D11)
#include "Direct3D11/D3D11ConstantBuffer.h"
#endif

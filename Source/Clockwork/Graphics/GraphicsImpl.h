

#pragma once

#if defined(CLOCKWORK_OPENGL)
#include "OpenGL/OGLGraphicsImpl.h"
#elif defined(CLOCKWORK_D3D11)
#include "Direct3D11/D3D11GraphicsImpl.h"
#else
#include "Direct3D9/D3D9GraphicsImpl.h"
#endif



#pragma once

#if defined(CLOCKWORK_OPENGL)
#include "OpenGL/OGLTexture3D.h"
#elif defined(CLOCKWORK_D3D11)
#include "Direct3D11/D3D11Texture3D.h"
#else
#include "Direct3D9/D3D9Texture3D.h"
#endif



#pragma once

#if defined(CLOCKWORK_OPENGL)
#include "OpenGL/OGLTextureCube.h"
#elif defined(CLOCKWORK_D3D11)
#include "Direct3D11/D3D11TextureCube.h"
#else
#include "Direct3D9/D3D9TextureCube.h"
#endif
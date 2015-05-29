#pragma once

#if defined(CLOCKWORK_OPENGL)
#include "OpenGL/OGLShaderProgram.h"
#elif defined(CLOCKWORK_D3D11)
#include "Direct3D11/D3D11ShaderProgram.h"
#else
#include "Direct3D9/D3D9ShaderProgram.h"
#endif

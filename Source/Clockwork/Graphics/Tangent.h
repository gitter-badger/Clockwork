

#pragma once

#ifdef CLOCKWORK_IS_BUILDING
#include "Clockwork.h"
#else
#include <Clockwork/Clockwork.h>
#endif

namespace Clockwork
{

/// Generate tangents to indexed geometry.
CLOCKWORK_API void GenerateTangents
    (void* vertexData, unsigned vertexSize, const void* indexData, unsigned indexSize, unsigned indexStart, unsigned indexCount,
        unsigned normalOffset, unsigned texCoordOffset, unsigned tangentOffset);

}

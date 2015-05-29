#pragma once

#include "../../Graphics/GraphicsDefs.h"
#include "../../Container/RefCounted.h"
#include "../../Container/Vector.h"

namespace Clockwork
{

class Graphics;
class ShaderVariation;
class VertexBuffer;

/// Vertex declaration.
class CLOCKWORK_API VertexDeclaration : public RefCounted
{
public:
    /// Construct with vertex buffers and element masks to base declaration on.
    VertexDeclaration(Graphics* graphics, ShaderVariation* vertexShader, VertexBuffer** buffers, unsigned* elementMasks);
    /// Destruct.
    ~VertexDeclaration();

    /// Return input layout object corresponding to the declaration.
    void* GetInputLayout() const { return inputLayout_; }

private:
    /// Input layout object.
    void* inputLayout_;
};

}

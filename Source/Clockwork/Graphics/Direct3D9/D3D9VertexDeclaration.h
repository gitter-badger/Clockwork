

#pragma once

#include "../../Container/RefCounted.h"
#include "../../Container/Vector.h"
#include "../../Graphics/GraphicsDefs.h"

#include <d3d9.h>

namespace Clockwork
{

class Graphics;
class VertexBuffer;

/// Element in the vertex declaration.
struct VertexDeclarationElement
{
    /// Stream index.
    unsigned stream_;
    /// Element type.
    VertexElement element_;
    /// Element offset.
    unsigned offset_;
};

/// Vertex declaration.
class CLOCKWORK_API VertexDeclaration : public RefCounted
{
public:
    /// Construct with vertex element mask.
    VertexDeclaration(Graphics* graphics, unsigned elementMask);
    /// Construct with vertex buffers and element masks to base declaration on.
    VertexDeclaration(Graphics* graphics, const PODVector<VertexBuffer*>& buffers, const PODVector<unsigned>& elementMasks);
    /// Construct with vertex buffers (shared pointer vector) and element masks to base declaration on.
    VertexDeclaration(Graphics* graphics, const Vector<SharedPtr<VertexBuffer> >& buffers, const PODVector<unsigned>& elementMasks);
    /// Destruct.
    ~VertexDeclaration();

    /// Return Direct3D vertex declaration.
    IDirect3DVertexDeclaration9* GetDeclaration() const { return declaration_; }

private:
    /// Create declaration.
    void Create(Graphics* graphics, const PODVector<VertexDeclarationElement>& elements);
    /// Release declaration.
    void Release();

    /// Direct3D vertex declaration.
    IDirect3DVertexDeclaration9* declaration_;
};

}

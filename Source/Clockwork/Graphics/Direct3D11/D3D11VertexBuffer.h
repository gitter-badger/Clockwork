#pragma once

#include "../../Graphics/GPUObject.h"
#include "../../Graphics/GraphicsDefs.h"
#include "../../Container/ArrayPtr.h"

namespace Clockwork
{

/// Hardware vertex buffer.
class CLOCKWORK_API VertexBuffer : public Object, public GPUObject
{
    OBJECT(VertexBuffer);

public:
    /// Construct.
    VertexBuffer(Context* context);
    /// Destruct.
    virtual ~VertexBuffer();

    /// Release buffer.
    virtual void Release();

    /// Enable shadowing in CPU memory. Shadowing is forced on if the graphics subsystem does not exist.
    void SetShadowed(bool enable);
    /// Set size and vertex elements and dynamic mode. Previous data will be lost.
    bool SetSize(unsigned vertexCount, unsigned elementMask, bool dynamic = false);
    /// Set all data in the buffer.
    bool SetData(const void* data);
    /// Set a data range in the buffer. Optionally discard data outside the range.
    bool SetDataRange(const void* data, unsigned start, unsigned count, bool discard = false);
    /// Lock the buffer for write-only editing. Return data pointer if successful. Optionally discard data outside the range.
    void* Lock(unsigned start, unsigned count, bool discard = false);
    /// Unlock the buffer and apply changes to the GPU buffer.
    void Unlock();

    /// Return whether CPU memory shadowing is enabled.
    bool IsShadowed() const { return shadowed_; }
    /// Return whether is dynamic.
    bool IsDynamic() const { return dynamic_; }
    /// Return whether is currently locked.
    bool IsLocked() const { return lockState_ != LOCK_NONE; }
    /// Return number of vertices.
    unsigned GetVertexCount() const {return vertexCount_; }
    /// Return vertex size.
    unsigned GetVertexSize() const { return vertexSize_; }
    /// Return bitmask of vertex elements.
    unsigned GetElementMask() const { return elementMask_; }
    /// Return offset of a specified element within a vertex.
    unsigned GetElementOffset(VertexElement element) const { return elementOffset_[element]; }
    /// Return buffer hash for building vertex declarations.
    unsigned long long GetBufferHash(unsigned streamIndex, unsigned useMask);
    /// Return CPU memory shadow data.
    unsigned char* GetShadowData() const { return shadowData_.Get(); }
    /// Return shared array pointer to the CPU memory shadow data.
    SharedArrayPtr<unsigned char> GetShadowDataShared() const { return shadowData_; }

    /// Return vertex size corresponding to a vertex element mask.
    static unsigned GetVertexSize(unsigned elementMask);
    /// Return element offset from an element mask.
    static unsigned GetElementOffset(unsigned elementMask, VertexElement element);

    /// Vertex element sizes.
    static const unsigned elementSize[];
    /// Vertex element semantic names.
    static const char* elementSemantics[];
    /// Vertex element semantic indices.
    static const unsigned elementSemanticIndices[];
    /// Vertex element formats.
    static const unsigned elementFormats[];

private:
    /// Update offsets of vertex elements.
    void UpdateOffsets();
    /// Create buffer.
    bool Create();
    /// Update the shadow data to the GPU buffer.
    bool UpdateToGPU();
    /// Map the GPU buffer into CPU memory.
    void* MapBuffer(unsigned start, unsigned count, bool discard);
    /// Unmap the GPU buffer.
    void UnmapBuffer();

    /// Shadow data.
    SharedArrayPtr<unsigned char> shadowData_;
    /// Number of vertices.
    unsigned vertexCount_;
    /// Vertex size.
    unsigned vertexSize_;
    /// Vertex element bitmask.
    unsigned elementMask_;
   /// Vertex element offsets.
    unsigned elementOffset_[MAX_VERTEX_ELEMENTS];
    /// Buffer locking state.
    LockState lockState_;
    /// Lock start vertex.
    unsigned lockStart_;
    /// Lock number of vertices.
    unsigned lockCount_;
    /// Scratch buffer for fallback locking.
    void* lockScratchData_;
    /// Dynamic flag.
    bool dynamic_;
    /// Shadowed flag.
    bool shadowed_;
};

}

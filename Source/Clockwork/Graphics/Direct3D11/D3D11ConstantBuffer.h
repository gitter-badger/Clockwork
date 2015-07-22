

#pragma once

#include "../../Container/ArrayPtr.h"
#include "../../Core/Object.h"
#include "../../Graphics/GPUObject.h"
#include "../../Graphics/GraphicsDefs.h"

namespace Clockwork
{

/// Hardware constant buffer.
class CLOCKWORK_API ConstantBuffer : public Object, public GPUObject
{
    OBJECT(ConstantBuffer);

public:
    /// Construct.
    ConstantBuffer(Context* context);
    /// Destruct.
    virtual ~ConstantBuffer();

    /// Release buffer.
    virtual void Release();

    /// Set size and create GPU-side buffer. Return true on success.
    bool SetSize(unsigned size);
    /// Set a generic parameter and mark buffer dirty.
    void SetParameter(unsigned offset, unsigned size, const void* data);
    /// Set a Vector3 array parameter and mark buffer dirty.
    void SetVector3ArrayParameter(unsigned offset, unsigned rows, const void* data);
    /// Apply to GPU.
    void Apply();

    /// Return size.
    unsigned GetSize() const { return size_; }

    /// Return whether has unapplied data.
    bool IsDirty() const { return dirty_; }

private:
    /// Shadow data.
    SharedArrayPtr<unsigned char> shadowData_;
    /// Buffer byte size.
    unsigned size_;
    /// Dirty flag.
    bool dirty_;
};

}

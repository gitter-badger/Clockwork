

#pragma once

#include "../../Container/Ptr.h"

namespace Clockwork
{

class Graphics;

/// Base class for GPU resources.
class CLOCKWORK_API GPUObject
{
public:
    /// Construct with graphics subsystem pointer.
    GPUObject(Graphics* graphics);
    /// Destruct. Remove from the graphics subsystem.
    virtual ~GPUObject();

    /// Unconditionally release the GPU resource.
    virtual void Release() { }

    /// Clear the data lost flag. No-op on D3D11.
    void ClearDataLost() { }

    /// Return the graphics subsystem.
    Graphics* GetGraphics() const;

    /// Return Direct3D object.
    void* GetGPUObject() const { return object_; }

    /// Return whether data is lost due to device loss. Always false on D3D11.
    bool IsDataLost() const { return false; }

    /// Return whether has pending data assigned while device was lost. Always false on D3D11.
    bool HasPendingData() const { return false; }

protected:
    /// Graphics subsystem.
    WeakPtr<Graphics> graphics_;
    /// Direct3D object.
    void* object_;
};

}

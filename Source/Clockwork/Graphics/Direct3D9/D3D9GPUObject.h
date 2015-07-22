

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

    /// Release default pool resources.
    virtual void OnDeviceLost() { }

    /// Recreate default pool resources.
    virtual void OnDeviceReset() { }

    /// Unconditionally release the GPU resource.
    virtual void Release() { }

    /// Clear the data lost flag.
    void ClearDataLost();

    /// Return the graphics subsystem.
    Graphics* GetGraphics() const;

    /// Return Direct3D object.
    void* GetGPUObject() const { return object_; }

    /// Return whether data is lost due to device loss.
    bool IsDataLost() const { return dataLost_; }

    /// Return whether has pending data assigned while device was lost.
    bool HasPendingData() const { return dataPending_; }

protected:
    /// Graphics subsystem.
    WeakPtr<Graphics> graphics_;
    /// Direct3D object.
    void* object_;
    /// Data lost flag.
    bool dataLost_;
    /// Data pending flag.
    bool dataPending_;
};

}

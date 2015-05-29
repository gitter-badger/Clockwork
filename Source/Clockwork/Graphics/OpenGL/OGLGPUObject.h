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
    /// Destruct. Remove from the Graphics.
    virtual ~GPUObject();

    /// Mark the GPU resource destroyed on context destruction.
    virtual void OnDeviceLost();
    /// Recreate the GPU resource and restore data if applicable.
    virtual void OnDeviceReset() {}
    /// Unconditionally release the GPU resource.
    virtual void Release() {}

    /// Clear the data lost flag.
    void ClearDataLost();

    /// Return the graphics subsystem.
    Graphics* GetGraphics() const;
    /// Return the object's OpenGL handle.
    unsigned GetGPUObject() const { return object_; }
    /// Return whether data is lost due to context loss.
    bool IsDataLost() const { return dataLost_; }
    /// Return whether has pending data assigned while context was lost.
    bool HasPendingData() const { return dataPending_; }

protected:
    /// Graphics subsystem.
    WeakPtr<Graphics> graphics_;
    /// Object handle.
    unsigned object_;
    /// Data lost flag.
    bool dataLost_;
    /// Data pending flag.
    bool dataPending_;
};

}

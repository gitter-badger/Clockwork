#include "../../Graphics/Graphics.h"
#include "../../Graphics/GraphicsImpl.h"
#include "../../Graphics/GPUObject.h"

#include "../../DebugNew.h"

namespace Clockwork
{

GPUObject::GPUObject(Graphics* graphics) :
    graphics_(graphics),
    object_(0)
{
    if (graphics_)
        graphics->AddGPUObject(this);
}

GPUObject::~GPUObject()
{
    if (graphics_)
        graphics_->RemoveGPUObject(this);
}

Graphics* GPUObject::GetGraphics() const
{
    return graphics_;
}

}

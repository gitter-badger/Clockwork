

#include "../../Precompiled.h"

#include "../../Graphics/Graphics.h"
#include "../../Graphics/GPUObject.h"

#include "../../DebugNew.h"

namespace Clockwork
{

GPUObject::GPUObject(Graphics* graphics) :
    graphics_(graphics),
    object_(0),
    dataLost_(false),
    dataPending_(false)
{
    if (graphics_)
        graphics->AddGPUObject(this);
}

GPUObject::~GPUObject()
{
    if (graphics_)
        graphics_->RemoveGPUObject(this);
}

void GPUObject::OnDeviceLost()
{
    object_ = 0;
}

void GPUObject::ClearDataLost()
{
    dataLost_ = false;
}

Graphics* GPUObject::GetGraphics() const
{
    return graphics_;
}

}

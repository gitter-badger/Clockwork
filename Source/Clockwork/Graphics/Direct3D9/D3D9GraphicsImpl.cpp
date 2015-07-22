

#include "../../Precompiled.h"

#include "../../Graphics/Graphics.h"
#include "../../Graphics/GraphicsImpl.h"

#include "../../DebugNew.h"

namespace Clockwork
{

GraphicsImpl::GraphicsImpl() :
    window_(0),
    interface_(0),
    device_(0),
    defaultColorSurface_(0),
    defaultDepthStencilSurface_(0),
    frameQuery_(0),
    adapter_(D3DADAPTER_DEFAULT),
    deviceType_(D3DDEVTYPE_HAL)
{
    memset(&presentParams_, 0, sizeof presentParams_);
}

bool GraphicsImpl::CheckFormatSupport(D3DFORMAT format, DWORD usage, D3DRESOURCETYPE type)
{
    return interface_ ? SUCCEEDED(interface_->CheckDeviceFormat(adapter_, deviceType_, D3DFMT_X8R8G8B8, usage, type, format)) :
        false;
}

}

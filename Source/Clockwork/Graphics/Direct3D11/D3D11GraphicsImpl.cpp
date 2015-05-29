#include "../../Graphics/Graphics.h"
#include "../../Graphics/GraphicsEvents.h"
#include "../../Graphics/GraphicsImpl.h"

#include "../../DebugNew.h"

namespace Clockwork
{

GraphicsImpl::GraphicsImpl() :
    window_(0),
    device_(0),
    deviceContext_(0),
    swapChain_(0),
    defaultRenderTargetView_(0),
    defaultDepthTexture_(0),
    defaultDepthStencilView_(0),
    depthStencilView_(0)
{
    for (unsigned i = 0; i < MAX_RENDERTARGETS; ++i)
        renderTargetViews_[i] = 0;

    for (unsigned i = 0; i < MAX_TEXTURE_UNITS; ++i)
    {
        shaderResourceViews_[i] = 0;
        samplers_[i] = 0;
    }

    for (unsigned i = 0; i < MAX_VERTEX_STREAMS; ++i)
    {
        vertexBuffers_[i] = 0;
        vertexSizes_[i] = 0;
        vertexOffsets_[i] = 0;
    }

    for (unsigned i = 0; i < MAX_SHADER_PARAMETER_GROUPS; ++i)
    {
        constantBuffers_[VS][i] = 0;
        constantBuffers_[PS][i] = 0;
    }
}

}

#pragma once

#include "../../Math/Color.h"
#include "../../Graphics/GraphicsDefs.h"

#include <d3d11.h>
#include <dxgi.h>
#include <SDL/SDL.h>

namespace Clockwork
{

/// %Graphics implementation. Holds API-specific objects.
class CLOCKWORK_API GraphicsImpl
{
    friend class Graphics;

public:
    /// Construct.
    GraphicsImpl();

    /// Return Direct3D device.
    ID3D11Device* GetDevice() const { return device_; }
    /// Return Direct3D immediate device context.
    ID3D11DeviceContext* GetDeviceContext() const { return deviceContext_; }
    /// Return swapchain.
    IDXGISwapChain* GetSwapChain() const { return swapChain_; }
    /// Return window.
    SDL_Window* GetWindow() const { return window_; }

private:
    /// SDL window.
    SDL_Window* window_;
    /// Graphics device.
    ID3D11Device* device_;
    /// Immediate device context.
    ID3D11DeviceContext* deviceContext_;
    /// Swap chain.
    IDXGISwapChain* swapChain_;
    /// Default (backbuffer) rendertarget view.
    ID3D11RenderTargetView* defaultRenderTargetView_;
    /// Default depth-stencil texture.
    ID3D11Texture2D* defaultDepthTexture_;
    /// Default depth-stencil view.
    ID3D11DepthStencilView* defaultDepthStencilView_;
    /// Current color rendertarget views.
    ID3D11RenderTargetView* renderTargetViews_[MAX_RENDERTARGETS];
    /// Current depth-stencil view.
    ID3D11DepthStencilView* depthStencilView_;
    /// Created blend state objects.
    HashMap<unsigned, ID3D11BlendState*> blendStates_;
    /// Created depth state objects.
    HashMap<unsigned, ID3D11DepthStencilState*> depthStates_;
    /// Created rasterizer state objects.
    HashMap<unsigned, ID3D11RasterizerState*> rasterizerStates_;
    /// Bound shader resource views.
    ID3D11ShaderResourceView* shaderResourceViews_[MAX_TEXTURE_UNITS];
    /// Bound sampler state objects.
    ID3D11SamplerState* samplers_[MAX_TEXTURE_UNITS];
    /// Bound vertex buffers.
    ID3D11Buffer* vertexBuffers_[MAX_VERTEX_STREAMS];
    /// Bound constant buffers.
    ID3D11Buffer* constantBuffers_[2][MAX_SHADER_PARAMETER_GROUPS];
    /// Vertex sizes per buffer.
    unsigned vertexSizes_[MAX_VERTEX_STREAMS];
    /// Vertex stream offsets per buffer.
    unsigned vertexOffsets_[MAX_VERTEX_STREAMS];
};

}

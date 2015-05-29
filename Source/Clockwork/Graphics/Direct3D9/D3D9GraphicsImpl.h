#pragma once

#include "../../Math/Color.h"

#include <d3d9.h>
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
    IDirect3DDevice9* GetDevice() const { return device_; }
    /// Return device capabilities.
    const D3DCAPS9& GetDeviceCaps() const { return deviceCaps_; }
    /// Return window.
    SDL_Window* GetWindow() const { return window_; }
    /// Return adapter identifier.
    const D3DADAPTER_IDENTIFIER9& GetAdapterIdentifier() const { return adapterIdentifier_; }
    /// Return whether a texture format and usage is supported.
    bool CheckFormatSupport(D3DFORMAT format, DWORD usage, D3DRESOURCETYPE type);

private:
    /// SDL window.
    SDL_Window* window_;
    /// Direct3D interface.
    IDirect3D9* interface_;
    /// Direct3D device.
    IDirect3DDevice9* device_;
    /// Default color surface.
    IDirect3DSurface9* defaultColorSurface_;
    /// Default depth-stencil surface.
    IDirect3DSurface9* defaultDepthStencilSurface_;
    /// Frame query for flushing the GPU command queue.
    IDirect3DQuery9* frameQuery_;
    /// Adapter number.
    DWORD adapter_;
    /// Device type.
    D3DDEVTYPE deviceType_;
    /// Device capabilities.
    D3DCAPS9 deviceCaps_;
    /// Adapter identifier.
    D3DADAPTER_IDENTIFIER9 adapterIdentifier_;
    /// Direct3D presentation parameters.
    D3DPRESENT_PARAMETERS presentParams_;
    /// Texture min/mag filter modes in use.
    D3DTEXTUREFILTERTYPE minMagFilters_[MAX_TEXTURE_UNITS];
    /// Texture mip filter modes in use.
    D3DTEXTUREFILTERTYPE mipFilters_[MAX_TEXTURE_UNITS];
    /// Texture U coordinate addressing modes in use.
    D3DTEXTUREADDRESS uAddressModes_[MAX_TEXTURE_UNITS];
    /// Texture V coordinate addressing modes in use.
    D3DTEXTUREADDRESS vAddressModes_[MAX_TEXTURE_UNITS];
    /// Texture W coordinate addressing modes in use.
    D3DTEXTUREADDRESS wAddressModes_[MAX_TEXTURE_UNITS];
    /// Texture border colors in use.
    Color borderColors_[MAX_TEXTURE_UNITS];
    /// sRGB mode in use.
    bool sRGBModes_[MAX_TEXTURE_UNITS];
    /// sRGB write flag.
    bool sRGBWrite_;
    /// Color surfaces in use.
    IDirect3DSurface9* colorSurfaces_[MAX_RENDERTARGETS];
    /// Depth-stencil surface in use.
    IDirect3DSurface9* depthStencilSurface_;
    /// Blending enabled flag.
    DWORD blendEnable_;
    /// Source blend mode.
    D3DBLEND srcBlend_;
    /// Destination blend mode.
    D3DBLEND destBlend_;
    /// Blend operation.
    D3DBLENDOP blendOp_;
};

}

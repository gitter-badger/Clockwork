

#pragma once

#include "../../Graphics/GraphicsDefs.h"
#include "../../Graphics/Viewport.h"

namespace Clockwork
{

class Texture;

/// %Color or depth-stencil surface that can be rendered into.
class CLOCKWORK_API RenderSurface : public RefCounted
{
    friend class Texture2D;
    friend class TextureCube;

public:
    /// Construct with parent texture.
    RenderSurface(Texture* parentTexture);
    /// Destruct.
    ~RenderSurface();

    /// Set number of viewports.
    void SetNumViewports(unsigned num);
    /// Set viewport.
    void SetViewport(unsigned index, Viewport* viewport);
    /// Set viewport update mode. Default is to update when visible.
    void SetUpdateMode(RenderSurfaceUpdateMode mode);
    /// Set linked color rendertarget.
    void SetLinkedRenderTarget(RenderSurface* renderTarget);
    /// Set linked depth-stencil surface.
    void SetLinkedDepthStencil(RenderSurface* depthStencil);
    /// Queue manual update of the viewport(s).
    void QueueUpdate();
    /// Release surface.
    void Release();

    /// Return parent texture.
    Texture* GetParentTexture() const { return parentTexture_; }

    /// Return Direct3D surface.
    void* GetSurface() const { return surface_; }

    /// Return width.
    int GetWidth() const;
    /// Return height.
    int GetHeight() const;
    /// Return usage.
    TextureUsage GetUsage() const;

    /// Return number of viewports.
    unsigned GetNumViewports() const { return viewports_.Size(); }

    /// Return viewport by index.
    Viewport* GetViewport(unsigned index) const;

    /// Return viewport update mode.
    RenderSurfaceUpdateMode GetUpdateMode() const { return updateMode_; }

    /// Return linked color rendertarget.
    RenderSurface* GetLinkedRenderTarget() const { return linkedRenderTarget_; }

    /// Return linked depth-stencil surface.
    RenderSurface* GetLinkedDepthStencil() const { return linkedDepthStencil_; }

    /// Clear update flag. Called by Renderer.
    void WasUpdated();

private:
    /// Parent texture.
    Texture* parentTexture_;
    /// Direct3D surface.
    void* surface_;
    /// Viewports.
    Vector<SharedPtr<Viewport> > viewports_;
    /// Linked color buffer.
    WeakPtr<RenderSurface> linkedRenderTarget_;
    /// Linked depth buffer.
    WeakPtr<RenderSurface> linkedDepthStencil_;
    /// Update mode for viewports.
    RenderSurfaceUpdateMode updateMode_;
    /// Update queued flag.
    bool updateQueued_;
};

}

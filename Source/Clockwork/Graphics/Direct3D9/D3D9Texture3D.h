

#pragma once

#include "../../Container/Ptr.h"
#include "../../Graphics/RenderSurface.h"
#include "../../Graphics/Texture.h"

namespace Clockwork
{

class Image;

/// 3D texture resource.
class CLOCKWORK_API Texture3D : public Texture
{
    OBJECT(Texture3D);

public:
    /// Construct.
    Texture3D(Context* context);
    /// Destruct.
    virtual ~Texture3D();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Load resource from stream. May be called from a worker thread. Return true if successful.
    virtual bool BeginLoad(Deserializer& source);
    /// Finish resource loading. Always called from the main thread. Return true if successful.
    virtual bool EndLoad();
    /// Release default pool resources.
    virtual void OnDeviceLost();
    /// Recreate default pool resources.
    virtual void OnDeviceReset();
    /// Release texture.
    virtual void Release();

    /// Set size, format and usage. Zero size will follow application window size. Return true if successful.
    bool SetSize(int width, int height, int depth, unsigned format, TextureUsage usage = TEXTURE_STATIC);
    /// Set data either partially or fully on a mip level. Return true if successful.
    bool SetData(unsigned level, int x, int y, int z, int width, int height, int depth, const void* data);
    /// Set data from an image. Return true if successful. Optionally make a single channel image alpha-only.
    bool SetData(SharedPtr<Image> image, bool useAlpha = false);

    /// Get data from a mip level. The destination buffer must be big enough. Return true if successful.
    bool GetData(unsigned level, void* dest) const;

    /// Return render surface.
    RenderSurface* GetRenderSurface() const { return renderSurface_; }

private:
    /// Create texture.
    bool Create();
    /// Handle render surface update event.
    void HandleRenderSurfaceUpdate(StringHash eventType, VariantMap& eventData);

    /// Render surface.
    SharedPtr<RenderSurface> renderSurface_;
    /// Image file acquired during BeginLoad.
    SharedPtr<Image> loadImage_;
    /// Parameter file acquired during BeginLoad.
    SharedPtr<XMLFile> loadParameters_;
};

}

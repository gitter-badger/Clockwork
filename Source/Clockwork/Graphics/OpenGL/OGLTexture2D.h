#pragma once

#include "../../Graphics/RenderSurface.h"
#include "../../Container/Ptr.h"
#include "../../Graphics/Texture.h"

namespace Clockwork
{

class Image;
class XMLFile;

/// 2D texture resource.
class CLOCKWORK_API Texture2D : public Texture
{
    OBJECT(Texture2D);

public:
    /// Construct.
    Texture2D(Context* context);
    /// Destruct.
    virtual ~Texture2D();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Load resource from stream. May be called from a worker thread. Return true if successful.
    virtual bool BeginLoad(Deserializer& source);
    /// Finish resource loading. Always called from the main thread. Return true if successful.
    virtual bool EndLoad();
    /// Mark the GPU resource destroyed on context destruction.
    virtual void OnDeviceLost();
    /// Recreate the GPU resource and restore data if applicable.
    virtual void OnDeviceReset();
    /// Release the texture.
    virtual void Release();

    /// Set size, format and usage. Zero size will follow application window size. Return true if successful.
    bool SetSize(int width, int height, unsigned format, TextureUsage usage = TEXTURE_STATIC);
    /// Set data either partially or fully on a mip level. Return true if successful.
    bool SetData(unsigned level, int x, int y, int width, int height, const void* data);
    /// Set data from an image. Return true if successful. Optionally make a single channel image alpha-only.
    bool SetData(SharedPtr<Image> image, bool useAlpha = false);

    /// Get data from a mip level. The destination buffer must be big enough. Return true if successful.
    bool GetData(unsigned level, void* dest) const;
    /// Return render surface.
    RenderSurface* GetRenderSurface() const { return renderSurface_; }

protected:
    /// Create texture.
    virtual bool Create();

private:
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



#pragma once

#include "../../Container/Ptr.h"
#include "../../Graphics/RenderSurface.h"
#include "../../Graphics/Texture.h"

namespace Clockwork
{

class Deserializer;
class Image;

/// Cube texture resource.
class CLOCKWORK_API TextureCube : public Texture
{
    OBJECT(TextureCube);

public:
    /// Construct.
    TextureCube(Context* context);
    /// Destruct.
    virtual ~TextureCube();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Load resource from stream. May be called from a worker thread. Return true if successful.
    virtual bool BeginLoad(Deserializer& source);
    /// Finish resource loading. Always called from the main thread. Return true if successful.
    virtual bool EndLoad();
    /// Release default pool resources.
    virtual void OnDeviceLost();
    /// ReCreate default pool resources.
    virtual void OnDeviceReset();
    /// Release texture.
    virtual void Release();

    /// Set size, format and usage. Return true if successful.
    bool SetSize(int size, unsigned format, TextureUsage usage = TEXTURE_STATIC);
    /// Set data either partially or fully on a face's mip level. Return true if successful.
    bool SetData(CubeMapFace face, unsigned level, int x, int y, int width, int height, const void* data);
    /// Set data of one face from a stream. Return true if successful.
    bool SetData(CubeMapFace face, Deserializer& source);
    /// Set data of one face from an image. Return true if successful. Optionally make a single channel image alpha-only.
    bool SetData(CubeMapFace face, SharedPtr<Image> image, bool useAlpha = false);

    /// Get data from a face's mip level. The destination buffer must be big enough. Return true if successful.
    bool GetData(CubeMapFace face, unsigned level, void* dest) const;

    /// Return render surface for one face.
    RenderSurface* GetRenderSurface(CubeMapFace face) const { return renderSurfaces_[face]; }

private:
    /// Create texture.
    bool Create();
    /// Handle render surface update event.
    void HandleRenderSurfaceUpdate(StringHash eventType, VariantMap& eventData);

    /// Render surfaces.
    SharedPtr<RenderSurface> renderSurfaces_[MAX_CUBEMAP_FACES];
    /// Memory use per face.
    unsigned faceMemoryUse_[MAX_CUBEMAP_FACES];
    /// Currently locked mip level.
    int lockedLevel_;
    /// Currently locked face.
    CubeMapFace lockedFace_;
    /// Face image files acquired during BeginLoad.
    Vector<SharedPtr<Image> > loadImages_;
    /// Parameter file acquired during BeginLoad.
    SharedPtr<XMLFile> loadParameters_;
};

}

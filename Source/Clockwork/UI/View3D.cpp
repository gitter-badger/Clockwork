#include "../Core/Context.h"
#include "../Graphics/Camera.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/Octree.h"
#include "../Scene/Scene.h"
#include "../Graphics/Texture2D.h"
#include "../Graphics/Viewport.h"
#include "../UI/View3D.h"
#include "../UI/UI.h"
#include "../UI/UIEvents.h"
#include "../Graphics/Zone.h"

namespace Clockwork
{

extern const char* UI_CATEGORY;

View3D::View3D(Context* context) :
    Window(context),
    ownScene_(true),
    rttFormat_(Graphics::GetRGBFormat()),
    autoUpdate_(true)
{
    renderTexture_ = new Texture2D(context_);
    depthTexture_ = new Texture2D(context_);
    viewport_ = new Viewport(context_);
}

View3D::~View3D()
{
    ResetScene();
}

void View3D::RegisterObject(Context* context)
{
    context->RegisterFactory<View3D>(UI_CATEGORY);

    COPY_BASE_ATTRIBUTES(Window);
    // The texture format is API specific, so do not register it as a serializable attribute
    ACCESSOR_ATTRIBUTE("Auto Update", GetAutoUpdate, SetAutoUpdate, bool, true, AM_FILE);
    UPDATE_ATTRIBUTE_DEFAULT_VALUE("Clip Children", true);
    UPDATE_ATTRIBUTE_DEFAULT_VALUE("Is Enabled", true);
}

void View3D::OnResize()
{
    int width = GetWidth();
    int height = GetHeight();

    if (width > 0 && height > 0)
    {
        renderTexture_->SetSize(width, height, rttFormat_, TEXTURE_RENDERTARGET);
        depthTexture_->SetSize(width, height, Graphics::GetDepthStencilFormat(), TEXTURE_DEPTHSTENCIL);
        RenderSurface* surface = renderTexture_->GetRenderSurface();
        surface->SetViewport(0, viewport_);
        surface->SetUpdateMode(autoUpdate_ ? SURFACE_UPDATEALWAYS : SURFACE_MANUALUPDATE);
        surface->SetLinkedDepthStencil(depthTexture_->GetRenderSurface());

        SetTexture(renderTexture_);
        SetImageRect(IntRect(0, 0, width, height));

        if (!autoUpdate_)
            surface->QueueUpdate();
    }
}

void View3D::SetView(Scene* scene, Camera* camera, bool ownScene)
{
    ResetScene();

    scene_ = scene;
    cameraNode_ = camera ? camera->GetNode() : 0;
    ownScene_ = ownScene;

    viewport_->SetScene(scene_);
    viewport_->SetCamera(camera);
    QueueUpdate();
}

void View3D::SetFormat(unsigned format)
{
    if (format != rttFormat_)
    {
        rttFormat_ = format;
        OnResize();
    }
}

void View3D::SetAutoUpdate(bool enable)
{
    if (enable != autoUpdate_)
    {
        autoUpdate_ = enable;
        RenderSurface* surface = renderTexture_->GetRenderSurface();
        if (surface)
            surface->SetUpdateMode(autoUpdate_ ? SURFACE_UPDATEALWAYS : SURFACE_MANUALUPDATE);
    }
}

void View3D::QueueUpdate()
{
    if (!autoUpdate_)
    {
        RenderSurface* surface = renderTexture_->GetRenderSurface();
        if (surface)
            surface->QueueUpdate();
    }
}

Scene* View3D::GetScene() const
{
    return scene_;
}

Node* View3D::GetCameraNode() const
{
    return cameraNode_;
}

Texture2D* View3D::GetRenderTexture() const
{
    return renderTexture_;
}

Texture2D* View3D::GetDepthTexture() const
{
    return depthTexture_;
}

Viewport* View3D::GetViewport() const
{
    return viewport_;
}

void View3D::ResetScene()
{
    if (!scene_)
        return;

    if (!ownScene_)
    {
        RefCount* refCount = scene_->RefCountPtr();
        ++refCount->refs_;
        scene_ = 0;
        --refCount->refs_;
    }
    else
        scene_ = 0;
}

}

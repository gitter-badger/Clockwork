#pragma once

#include "../Core/Object.h"
#include "../Container/Ptr.h"
#include "../Math/Ray.h"
#include "../Math/Rect.h"
#include "../Math/Vector2.h"

namespace Clockwork
{

class Camera;
class RenderPath;
class Scene;
class XMLFile;
class View;

/// %Viewport definition either for a render surface or the backbuffer.
class CLOCKWORK_API Viewport : public Object
{
    OBJECT(Viewport);

public:
    /// Construct with defaults.
    Viewport(Context* context);
    /// Construct with a full rectangle.
    Viewport(Context* context, Scene* scene, Camera* camera, RenderPath* renderPath = 0);
    /// Construct with a specified rectangle.
    Viewport(Context* context, Scene* scene, Camera* camera, const IntRect& rect, RenderPath* renderPath = 0);
    /// Destruct.
    ~Viewport();

    /// Set scene.
    void SetScene(Scene* scene);
    /// Set camera.
    void SetCamera(Camera* camera);
    /// Set rectangle.
    void SetRect(const IntRect& rect);
    /// Set rendering path.
    void SetRenderPath(RenderPath* path);
    /// Set rendering path from an XML file.
    void SetRenderPath(XMLFile* file);
    /// Set whether to render debug geometry. Default true.
    void SetDrawDebug(bool enable);

    /// Return scene.
    Scene* GetScene() const;
    /// Return camera.
    Camera* GetCamera() const;
    /// Return the internal rendering structure. May be null if the viewport has not been rendered yet.
    View* GetView() const;
    /// Return rectangle.
    const IntRect& GetRect() const { return rect_; }
    /// Return rendering path.
    RenderPath* GetRenderPath() const;
    /// Return whether to draw debug geometry.
    bool GetDrawDebug() const { return drawDebug_; }
    /// Return ray corresponding to normalized screen coordinates.
    Ray GetScreenRay(int x, int y) const;
    // Convert a world space point to normalized screen coordinates.
    IntVector2 WorldToScreenPoint(const Vector3& worldPos) const;
    // Convert screen coordinates and depth to a world space point.
    Vector3 ScreenToWorldPoint(int x, int y, float depth) const;

    /// Allocate the view structure. Called by Renderer.
    void AllocateView();

private:
    /// Scene pointer.
    WeakPtr<Scene> scene_;
    /// Camera pointer.
    WeakPtr<Camera> camera_;
    /// Viewport rectangle.
    IntRect rect_;
    /// Rendering path.
    SharedPtr<RenderPath> renderPath_;
    /// Internal rendering structure.
    SharedPtr<View> view_;
    /// Debug draw flag.
    bool drawDebug_;
};

}

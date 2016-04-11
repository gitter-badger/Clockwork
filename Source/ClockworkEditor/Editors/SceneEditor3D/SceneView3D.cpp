// Portions Copyright (c) 2008-2015 the Urho3D project.

//
// Copyright (c) 2014-2016 CLOCKWORK STUDIOS
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include <Clockwork/IO/Log.h>
#include <Clockwork/Core/CoreEvents.h>
#include <Clockwork/Scene/SceneEvents.h>
#include <Clockwork/Scene/Scene.h>
#include <Clockwork/Scene/PrefabComponent.h>
#include <Clockwork/Graphics/Camera.h>

#include <Clockwork/Graphics/Graphics.h>
#include <Clockwork/Graphics/DebugRenderer.h>
#include <Clockwork/Graphics/Viewport.h>
#include <Clockwork/Graphics/Octree.h>
#include <Clockwork/Graphics/Material.h>

#include <Clockwork/Clockwork3D/Model.h>
#include <Clockwork/Clockwork3D/StaticModel.h>
#include <Clockwork/Clockwork3D/AnimatedModel.h>
#include <Clockwork/Clockwork3D/AnimationController.h>

#include <Clockwork/Input/Input.h>

#include <Clockwork/IO/FileSystem.h>
#include <Clockwork/Resource/ResourceCache.h>
#include <Clockwork/Resource/XMLFile.h>
#include <Clockwork/Physics/PhysicsWorld.h>
#include <Clockwork/UI/UI.h>
#include <Clockwork/UI/UIEvents.h>

#include <Clockwork/Resource/ResourceEvents.h>

#include <ToolCore/Assets/Asset.h>
#include <ToolCore/Assets/AssetDatabase.h>

#include "../../EditorMode/CEEditorEvents.h"

#include "SceneView3D.h"
#include "SceneEditor3D.h"
#include "SceneEditor3DEvents.h"
#include "SceneSelection.h"

using namespace ToolCore;

namespace ClockworkEditor
{

SceneView3D ::SceneView3D(Context* context, SceneEditor3D *sceneEditor) :
    UISceneView(context),
    yaw_(0.0f),
    pitch_(0.0f),
    mouseLeftDown_(false),
    mouseMoved_(false),
    enabled_(true),
    cameraMove_(false)
{

    sceneEditor_ = sceneEditor;

    ResourceCache* cache = GetSubsystem<ResourceCache>();

    scene_ = sceneEditor->GetScene();

    debugRenderer_ = scene_->GetComponent<DebugRenderer>();

    if (debugRenderer_.Null())
    {
        debugRenderer_ = scene_->CreateComponent<DebugRenderer>();
        debugRenderer_->SetTemporary(true);
    }

    octree_ = scene_->GetComponent<Octree>();

    if (octree_.Null())
    {
        LOGWARNING("Scene without an octree loaded");
        octree_ = scene_->CreateComponent<Octree>();
    }

    cameraNode_ = scene_->CreateChild("__clockwork_sceneview3d_camera");
    cameraNode_->SetTemporary(true);
    camera_ = cameraNode_->CreateComponent<Camera>();

    debugRenderer_ = scene_->GetComponent<DebugRenderer>();
    assert(debugRenderer_.NotNull());
    octree_ = scene_->GetComponent<Octree>();
    assert(octree_.NotNull());

    cameraNode_->SetPosition(Vector3(0, 0, -10));

    SetView(scene_, camera_);
    SetAutoUpdate(false);

    SubscribeToEvent(E_UPDATE, HANDLER(SceneView3D, HandleUpdate));
    SubscribeToEvent(E_POSTRENDERUPDATE, HANDLER(SceneView3D, HandlePostRenderUpdate));

    SubscribeToEvent(E_MOUSEMOVE, HANDLER(SceneView3D,HandleMouseMove));

    SubscribeToEvent(this, E_DRAGENTERWIDGET, HANDLER(SceneView3D, HandleDragEnterWidget));
    SubscribeToEvent(this, E_DRAGEXITWIDGET, HANDLER(SceneView3D, HandleDragExitWidget));
    SubscribeToEvent(this, E_DRAGENDED, HANDLER(SceneView3D, HandleDragEnded));

    SubscribeToEvent(E_UIUNHANDLEDSHORTCUT, HANDLER(SceneView3D, HandleUIUnhandledShortcut));
    SubscribeToEvent(E_UIWIDGETFOCUSESCAPED, HANDLER(SceneView3D, HandleUIWidgetFocusEscaped));

    SetIsFocusable(true);

}

SceneView3D::~SceneView3D()
{

}

void SceneView3D::Enable()
{
    if (enabled_)
        return;

    enabled_ = true;

}

void SceneView3D::Disable()
{
    if (!enabled_)
        return;

    enabled_ = false;

}

bool SceneView3D::GetOrbitting()
{
    Input* input = GetSubsystem<Input>();
    return framedBBox_.defined_ && MouseInView() && input->GetKeyDown(KEY_ALT) && input->GetMouseButtonDown(MOUSEB_LEFT);
}

bool SceneView3D::GetZooming()
{
    Input* input = GetSubsystem<Input>();
    return MouseInView() && input->GetKeyDown(KEY_ALT) && input->GetMouseMoveWheel();
}


void SceneView3D::MoveCamera(float timeStep)
{
    if (!enabled_ && !GetFocus())
        return;

    Input* input = GetSubsystem<Input>();
    bool shiftDown = false;
    if (input->GetKeyDown(KEY_LSHIFT) || input->GetKeyDown(KEY_RSHIFT))
        shiftDown = true;

    bool mouseInView = MouseInView();
    bool orbitting = GetOrbitting();
    bool zooming = GetZooming();

    // Movement speed as world units per second
    float MOVE_SPEED = 20.0f;
    // Mouse sensitivity as degrees per pixel
    const float MOUSE_SENSITIVITY = 0.2f;

    if (shiftDown)
        MOVE_SPEED *= 3.0f;

    // Use this frame's mouse motion to adjust camera node yaw and pitch. Clamp the pitch between -90 and 90 degrees
    if ((mouseInView && input->GetMouseButtonDown(MOUSEB_RIGHT)) || orbitting)
    {
        SetFocus();
        IntVector2 mouseMove = input->GetMouseMove();
        yaw_ += MOUSE_SENSITIVITY * mouseMove.x_;
        pitch_ += MOUSE_SENSITIVITY * mouseMove.y_;
        pitch_ = Clamp(pitch_, -90.0f, 90.0f);
    }

    // Construct new orientation for the camera scene node from yaw and pitch. Roll is fixed to zero
    Quaternion q(pitch_, yaw_, 0.0f);

    if (!zooming)
        cameraNode_->SetRotation(q);

    if (orbitting)
    {
        BoundingBox bbox;
        sceneEditor_->GetSelection()->GetBounds(bbox);
        if (bbox.defined_)
        {
            Vector3 centerPoint = bbox.Center();
            Vector3 d = cameraNode_->GetWorldPosition() - centerPoint;
            cameraNode_->SetWorldPosition(centerPoint - q * Vector3(0.0, 0.0, d.Length()));

        }
    }

    if (zooming)
    {
        Ray ray = GetCameraRay();
        Vector3 wpos = cameraNode_->GetWorldPosition();
        wpos += ray.direction_ * (float (input->GetMouseMoveWheel()) * (shiftDown ? 0.6f : 0.2f));
        cameraNode_->SetWorldPosition(wpos);
    }


#ifdef CLOCKWORK_PLATFORM_WINDOWS
    bool superdown = input->GetKeyDown(KEY_LCTRL) || input->GetKeyDown(KEY_RCTRL);
#else
    bool superdown = input->GetKeyDown(KEY_LGUI) || input->GetKeyDown(KEY_RGUI);
#endif

    if (!orbitting && mouseInView && !superdown && input->GetMouseButtonDown(MOUSEB_RIGHT)) {

        // Read WASD keys and move the camera scene node to the corresponding direction if they are pressed
        // Use the Translate() function (default local space) to move relative to the node's orientation.
        if (input->GetKeyDown(KEY_W))
        {
            SetFocus();
            cameraNode_->Translate(Vector3::FORWARD * MOVE_SPEED * timeStep);
        }
        if (input->GetKeyDown(KEY_S))
        {
            SetFocus();
            cameraNode_->Translate(Vector3::BACK * MOVE_SPEED * timeStep);
        }
        if (input->GetKeyDown(KEY_A))
        {   SetFocus();
            cameraNode_->Translate(Vector3::LEFT * MOVE_SPEED * timeStep);
        }
        if (input->GetKeyDown(KEY_D))
        {
            SetFocus();
            cameraNode_->Translate(Vector3::RIGHT * MOVE_SPEED * timeStep);
        }
        if (input->GetKeyDown(KEY_Q))
        {
            SetFocus();
            cameraNode_->Translate(Vector3::UP * MOVE_SPEED * timeStep);
        }
        if (input->GetKeyDown(KEY_E))
        {
            SetFocus();
            cameraNode_->Translate(Vector3::DOWN * MOVE_SPEED * timeStep);
        }
    }

    if (cameraMove_)
    {

        cameraMoveTime_ += timeStep * 3.0f;

        if (cameraMoveTime_ > 1.0f)
        {
            cameraMove_ = false;
            cameraMoveTime_ = 1.0f;
        }

        Vector3 pos = cameraMoveStart_.Lerp(cameraMoveTarget_, cameraMoveTime_);
        cameraNode_->SetWorldPosition(pos);

    }
}

Ray SceneView3D::GetCameraRay()
{
    Ray camRay;

    Input* input = GetSubsystem<Input>();
    IntVector2 cpos = input->GetMousePosition();

    IntRect rect = GetRect();

    if (!rect.Width() || !rect.Height())
        return camRay;

    int x = rect.left_;
    int y = rect.top_;
    GetInternalWidget()->ConvertToRoot(x, y);

    return  camera_->GetScreenRay(float(cpos.x_ - x) / rect.Width(),
                                  float(cpos.y_ - y) / rect.Height());
}

bool SceneView3D::MouseInView()
{
    if (!GetInternalWidget())
        return false;

    if (!TBWidget::hovered_widget || TBWidget::hovered_widget->GetDelegate() != this)
        return false;

    Input* input = GetSubsystem<Input>();
    IntVector2 pos = input->GetMousePosition();

    IntRect rect = GetRect();

    GetInternalWidget()->ConvertToRoot(rect.left_, rect.top_);
    GetInternalWidget()->ConvertToRoot(rect.right_, rect.bottom_);

    return rect.IsInside(pos);

}

void SceneView3D::HandleUIUnhandledShortcut(StringHash eventType, VariantMap& eventData)
{
    if (!enabled_)
        return;

    unsigned id = eventData[UIUnhandledShortcut::P_REFID].GetUInt();

    if (id == TBIDC("undo"))
        sceneEditor_->Undo();
    else if (id == TBIDC("redo"))
        sceneEditor_->Redo();
    else if (id == TBIDC("copy"))
        sceneEditor_->Copy();
    else if (id == TBIDC("cut"))
        sceneEditor_->Cut();
    else if (id == TBIDC("paste"))
        sceneEditor_->Paste();

    return;

}

void SceneView3D::HandleUIWidgetFocusEscaped(StringHash eventType, VariantMap& eventData)
{
    if (!enabled_)
        return;

    SetFocus();
}

void SceneView3D::HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData)
{

    if (!MouseInView() || GetOrbitting())
        return;

    Input* input = GetSubsystem<Input>();

    mouseLeftDown_ = false;
    bool shiftDown = input->GetKeyDown(KEY_LSHIFT) || input->GetKeyDown(KEY_RSHIFT);

    if (input->GetMouseButtonPress(MOUSEB_LEFT))
    {
        SetFocus();

        if (!mouseMoved_ && !sceneEditor_->GetGizmo()->Selected())
        {
            Ray camRay  = GetCameraRay();
            PODVector<RayQueryResult> result;

            RayOctreeQuery query(result, camRay, RAY_TRIANGLE, camera_->GetFarClip(), DRAWABLE_GEOMETRY, 0x7fffffff);
            octree_->RaycastSingle(query);

            if (query.result_.Size())
            {
                const RayQueryResult& r = result[0];

                if (r.drawable_)
                {

                    VariantMap neventData;
                    Node* node = r.drawable_->GetNode();

                    // if temporary, this is a prefab
                    // TODO: if we use temporary for other stuff
                    // fix this to look for prefab
                    if (node->IsTemporary())
                        node = node->GetParent();

                    if (sceneEditor_->GetSelection()->Contains(node) && shiftDown)
                    {
                        sceneEditor_->GetSelection()->RemoveNode(node);
                    }
                    else
                    {
                        sceneEditor_->GetSelection()->AddNode(node, !shiftDown);
                    }

                }
            }
            else
            {
                sceneEditor_->GetSelection()->Clear();
            }
        }

        mouseMoved_ = false;

    }
    else if (!input->GetMouseButtonDown(MOUSEB_LEFT))
    {

        Ray camRay  = GetCameraRay();
        PODVector<RayQueryResult> result;

        mouseMoved_ = false;

        /*
        Array<int> pickModeDrawableFlags = {
            DRAWABLE_GEOMETRY,
            DRAWABLE_LIGHT,
            DRAWABLE_ZONE
        };
        */

        RayOctreeQuery query(result, camRay, RAY_TRIANGLE, camera_->GetFarClip(), DRAWABLE_GEOMETRY, 0x7fffffff);
        octree_->RaycastSingle(query);

        if (query.result_.Size())
        {
            const RayQueryResult& r = result[0];

            if (r.drawable_)
            {
                debugRenderer_->AddNode(r.drawable_->GetNode(), 1.0, false);
                r.drawable_->DrawDebugGeometry(debugRenderer_, false);
            }
        }
    }
    else
    {
        mouseLeftDown_ = true;
        if (Abs(input->GetMouseMoveX() > 3 || input->GetMouseMoveY() >  3))
        {
            mouseMoved_ = true;
        }
    }

}

bool SceneView3D::OnEvent(const TBWidgetEvent &ev)
{
    if (ev.type == EVENT_TYPE_SHORTCUT)
    {
        if (ev.ref_id == TBIDC("close"))
            return false;
    }
    if (ev.type == EVENT_TYPE_KEY_UP)
    {
        if (ev.special_key == TB_KEY_ESC)
        {
            sceneEditor_->GetSelection()->Clear();
        }
    }

    return sceneEditor_->OnEvent(ev);
}


void SceneView3D::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    // parent is the contentRoot for our tab, when tab isn't active it will not be visible
    if (!GetParent() || GetParent()->GetVisibility() != UI_WIDGET_VISIBILITY_VISIBLE)
    {
        Disable();
        return;
    }

    Enable();

    // Timestep parameter is same no matter what event is being listened to
    float timeStep = eventData[Update::P_TIMESTEP].GetFloat();

    MoveCamera(timeStep);

    QueueUpdate();

    if (preloadResourceScene_.NotNull())
    {
        if (preloadResourceScene_->GetAsyncProgress() == 1.0f)
        {
            ResourceCache* cache = GetSubsystem<ResourceCache>();
            XMLFile* xml = cache->GetResource<XMLFile>(dragAssetGUID_);

            if (dragNode_.NotNull())
            {
                dragNode_->LoadXML(xml->GetRoot());
                UpdateDragNode(0, 0);

                AnimationController* controller = dragNode_->GetComponent<AnimationController>();
                if (controller)
                {
                    controller->PlayExclusive("Idle", 0, true);

                    dragNode_->GetScene()->SetUpdateEnabled(true);
                }
            }

            preloadResourceScene_ = 0;
            dragAssetGUID_ = "";

        }
    }

}


void SceneView3D::UpdateDragNode(int mouseX, int mouseY)
{
    if (dragNode_.Null())
        return;

    Ray ray = GetCameraRay();

    Vector3 pos = ray.origin_;
    pos += ray.direction_ * 10;

    dragNode_->SetWorldPosition(pos);

}

void SceneView3D::HandleMouseMove(StringHash eventType, VariantMap& eventData)
{
    if (dragNode_.Null())
        return;

    Input* input = GetSubsystem<Input>();

    if (!input->IsMouseVisible())
        return;

    using namespace MouseMove;

    int x = eventData[P_X].GetInt();
    int y = eventData[P_Y].GetInt();

    UpdateDragNode(x, y);

}

void SceneView3D::HandleDragEnterWidget(StringHash eventType, VariantMap& eventData)
{
    using namespace DragEnterWidget;

    UIWidget* widget = static_cast<UIWidget*>(eventData[P_WIDGET].GetPtr());

    if (widget != this)
        return;

    UIDragObject* dragObject = static_cast<UIDragObject*>(eventData[P_DRAGOBJECT].GetPtr());

    Object* object = dragObject->GetObject();

    if (!object)
        return;

    if (object->GetType() == Asset::GetTypeStatic())
    {
        Asset* asset = (Asset*) object;
        dragNode_ = asset->InstantiateNode(scene_, asset->GetName());

        if (dragNode_.NotNull())
        {
            Input* input = GetSubsystem<Input>();
            IntVector2 pos = input->GetMousePosition();
            UpdateDragNode(pos.x_, pos.y_);
        }

        //LOGINFOF("Dropped %s : %s on SceneView3D", asset->GetPath().CString(), asset->GetGUID().CString());
    }

}

void SceneView3D::HandleDragExitWidget(StringHash eventType, VariantMap& eventData)
{
    if (preloadResourceScene_.NotNull())
    {
        preloadResourceScene_->StopAsyncLoading();
        preloadResourceScene_ = 0;
    }

    if (dragNode_.NotNull())
    {
        scene_->RemoveChild(dragNode_);
    }

    dragAssetGUID_ = "";
    dragNode_ = 0;
}


void SceneView3D::HandleDragEnded(StringHash eventType, VariantMap& eventData)
{
    using namespace DragEnded;

    UIDragObject* dragObject = static_cast<UIDragObject*>(eventData[P_DRAGOBJECT].GetPtr());

    if (dragNode_.NotNull())
    {
        VariantMap nodeCreatedEvent;
        nodeCreatedEvent[SceneEditNodeCreated::P_NODE] = dragNode_;
        scene_->SendEvent(E_SCENEEDITNODECREATED, nodeCreatedEvent);
    }

    if (dragObject && dragObject->GetObject()->GetType() == ToolCore::Asset::GetTypeStatic())
    {
        Asset* asset = (ToolCore::Asset*) dragObject->GetObject();

        if (asset->GetImporterTypeName() == "MaterialImporter") {

            Material* material = GetSubsystem<ResourceCache>()->GetResource<Material>(asset->GetPath());

            if (material) {

                material = material;

                Ray camRay  = GetCameraRay();

                PODVector<RayQueryResult> result;

                RayOctreeQuery query(result, camRay, RAY_TRIANGLE, camera_->GetFarClip(), DRAWABLE_GEOMETRY, 0x7fffffff);
                octree_->RaycastSingle(query);

                if (query.result_.Size())
                {
                    const RayQueryResult& r = result[0];

                    if (r.drawable_ && (r.drawable_->GetType() == StaticModel::GetTypeStatic() || r.drawable_->GetType() == AnimatedModel::GetTypeStatic()))
                    {
                        ((StaticModel*)r.drawable_)->SetMaterial(material);

                    }
                }

            }

        }

    }

    dragAssetGUID_ = "";
    dragNode_ = 0;
}

void SceneView3D::FrameSelection()
{
    BoundingBox bbox;
    sceneEditor_->GetSelection()->GetBounds(bbox);

    if (!bbox.defined_)
        return;

    Sphere sphere(bbox);

    if (sphere.radius_ < .01f || sphere.radius_ > 512)
        return;

    framedBBox_ = bbox;
    cameraMoveStart_ = cameraNode_->GetWorldPosition();
    cameraMoveTarget_ = bbox.Center() - (cameraNode_->GetWorldDirection() * sphere.radius_ * 3);
    cameraMoveTime_ = 0.0f;
    cameraMove_ = true;

}


}

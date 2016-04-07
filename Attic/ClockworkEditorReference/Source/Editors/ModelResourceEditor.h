#pragma once

#include <TurboBadger/tb_widgets_common.h>
#include "ResourceEditor.h"

using namespace Clockwork;
using namespace tb;

namespace Clockwork
{
class Scene;
class Node;
class Camera;
}

namespace ClockworkEditor
{

class JSAutocomplete;

class ModelResourceEditor: public ResourceEditor
{
    OBJECT(ModelResourceEditor);

public:

    ModelResourceEditor(Context* context, const String& fullpath, TBTabContainer* container);

    virtual ~ModelResourceEditor();

    bool OnEvent(const TBWidgetEvent &ev);

    void HandleUpdate(StringHash eventType, VariantMap& eventData);

private:

    void MoveCamera(float timeStep);

    SharedPtr<Scene> scene_;
    SharedPtr<Node> cameraNode_;
    //SharedPtr<View3D> view3D_;
    SharedPtr<Camera> camera_;
    float yaw_;
    float pitch_;

    TBLayout* layout_;
    TBContainer* view3DContainer_;

    WeakPtr<Node> modelNode_;

};

}

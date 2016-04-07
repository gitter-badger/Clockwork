#include "ClockworkEditor.h"
#include <Clockwork/Core/CoreEvents.h>
#include <Clockwork/Scene/Scene.h>
#include <Clockwork/Graphics/Octree.h>
#include <Clockwork/Graphics/Camera.h>
#include <Clockwork/Graphics/Zone.h>
#include <Clockwork/Clockwork3D/AnimatedModel.h>
#include <Clockwork/Clockwork3D/Animation.h>
#include <Clockwork/Clockwork3D/AnimationState.h>
#include <Clockwork/Clockwork3D/AnimationController.h>
#include <Clockwork/Graphics/Material.h>
#include <Clockwork/Graphics/DebugRenderer.h>

#include <Clockwork/Resource/ResourceCache.h>
#include <Clockwork/Input/Input.h>

#include "../AEEditor.h"


#include "ModelResourceEditor.h"

#include <Clockwork/UI/UI.h>

namespace ClockworkEditor
{

ModelResourceEditor ::ModelResourceEditor(Context* context, const String &fullpath, TBTabContainer *container) :
    ResourceEditor(context, fullpath, container),
    layout_(0),
    view3DContainer_(0),
    yaw_(0.0f),
    pitch_(0.0f)
{
}

ModelResourceEditor::~ModelResourceEditor()
{

}

bool ModelResourceEditor::OnEvent(const TBWidgetEvent &ev)
{

    return false;
}

void ModelResourceEditor::MoveCamera(float timeStep)
{

}

void ModelResourceEditor::HandleUpdate(StringHash eventType, VariantMap& eventData)
{



}


}

#pragma once

#include "AEWidget.h"
#include "UIMenubar.h"

#include <Clockwork/Scene/Scene.h>

using namespace Clockwork;

namespace tb
{
    class TBLayout;
    class TBMenuWindow;
}

namespace ClockworkEditor
{

class ListView;
class ListViewItem;

class HierarchyFrame : public AEWidget
{

    OBJECT(HierarchyFrame);

public:
    /// Construct.
    HierarchyFrame(Context* context);
    /// Destruct.
    virtual ~HierarchyFrame();

    void RefreshHierarchyList();

    bool OnEvent(const TBWidgetEvent &ev);

private:
    void HandleEditorActiveSceneChange(StringHash eventType, VariantMap& eventData);

    void RecursiveAddNode(ListViewItem* parent, Node* node);

    SharedPtr<ListView> hierarchyList_;

    SharedPtr<Scene> scene_;
};

}

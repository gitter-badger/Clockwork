// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#pragma once

#include "CEWidget.h"
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

class HierarchyFrame : public CEWidget
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

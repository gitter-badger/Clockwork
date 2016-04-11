// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#pragma once

#include "CEWidget.h"

using namespace Clockwork;
using namespace tb;

namespace tb
{
class TBLayout;
}

namespace ClockworkEditor
{

class ListView;

class IssuesWidget: public CEWidget
{
    OBJECT(IssuesWidget);

public:

    IssuesWidget(Context* context);
    virtual ~IssuesWidget();

    void UpdateIssues();

    bool OnEvent(const TBWidgetEvent &ev);

private:

    void HandleEditorShutdown(StringHash eventType, VariantMap& eventData);

    SharedPtr<ListView> issueList_;
    TBLayout* issuelayout_;

};

}

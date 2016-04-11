// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#pragma once

#include "CEWidget.h"
#include <TurboBadger/tb_style_edit.h>

using namespace Clockwork;
using namespace tb;

namespace tb
{
class TBEditField;
}

namespace ClockworkEditor
{


class FindTextWidget: public CEWidget
{
    OBJECT(FindTextWidget);

public:

    FindTextWidget(Context* context);
    virtual ~FindTextWidget();

    void SetUISize(int uiWidth, int uiHeight);

    bool OnEvent(const TBWidgetEvent &ev);

    void Find(const String& findText, unsigned flags);

    void GetFindText(String& text);

    void HandleFindTextOpen(StringHash eventType, VariantMap& eventData);
    void HandleFindTextClose(StringHash eventType, VariantMap& eventData);

private:

    void HandleEditorShutdown(StringHash eventType, VariantMap& eventData);

    TBEditField* findtextedit_;

};

}

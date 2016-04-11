// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#pragma once

#include <UI/CEWidget.h>

namespace tb
{
    class TBWindow;
    class TBDimmer;
    class TBTextField;
}

using namespace Clockwork;
using namespace tb;

namespace ClockworkEditor
{

class ProgressModal : public CEWidget
{
    OBJECT(ProgressModal);

public:
    /// Construct.
    ProgressModal(Context* context, const String& title, const String& message);
    /// Destruct.
    virtual ~ProgressModal();

    void Show();
    void Hide();
    void Center();

    void SetMessage(const String& message);

private:

    TBWindow* window_;
    TBDimmer* dimmer_;
    TBTextField* message_;

};


}

// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#pragma once

#include <Clockwork/Container/Str.h>
#include <Clockwork/Container/HashMap.h>
#include <Clockwork/Container/List.h>
#include <Clockwork/Math/StringHash.h>

#include <TurboBadger/tb_select.h>
#include <TurboBadger/tb_editfield.h>
#include <TurboBadger/tb_style_edit.h>

using namespace Clockwork;

using namespace tb;

namespace ClockworkEditor
{

// per document autocompletion
class JSAutocomplete
{
public:

    JSAutocomplete(TBEditField* editField);

    ~JSAutocomplete();

    void Show();
    void Hide();
    bool Visible();

    void SetPosition(const TBPoint& pos);
    bool UpdateCompletions(const String& value);

    bool OnEvent(const TBWidgetEvent &ev);

    void UpdateLocals();

private:

    HashMap<StringHash, List<String> > locals_;

    TBEditField* editField_;
    TBStyleEdit* styleEdit_;
    TBSelectList* autoList_;
    TBGenericStringItemSource autoSource_;
    String currentValue_;

};


}

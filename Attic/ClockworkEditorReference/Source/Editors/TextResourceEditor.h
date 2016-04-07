#pragma once

#include "ResourceEditor.h"
#include <TurboBadger/tb_editfield.h>
#include <TurboBadger/tb_style_edit.h>
#include <TurboBadger/tb_select.h>

using namespace Clockwork;
using namespace tb;

namespace ClockworkEditor
{

class TextResourceEditor: public ResourceEditor, public TBStyleEditTextChangeListener
{
    OBJECT(TextResourceEditor);

public:

    TextResourceEditor(Context* context, const String& fullpath, TBTabContainer* container);

    virtual ~TextResourceEditor();

    bool OnEvent(const TBWidgetEvent &ev);

    bool FindText(const String& findText, unsigned flags);
    void FindTextClose();

    void SetFocus();

    bool HasUnsavedModifications();
    void OnChange(TBStyleEdit* styleEdit);

private:

    void HandleUpdate(StringHash eventType, VariantMap& eventData);

    void UpdateLineNumbers();

    tb::TBStyleEdit* styleEdit_;
    TBEditField* editField_;

    bool modified_;

    int currentFindPos_;

};

}

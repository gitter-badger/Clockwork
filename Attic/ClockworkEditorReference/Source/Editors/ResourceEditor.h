#pragma once

#include <Clockwork/Core/Object.h>
#include <TurboBadger/tb_tab_container.h>

using namespace Clockwork;
using namespace tb;

namespace ClockworkEditor
{

class EditorTabLayout;

class ResourceEditor: public Object
{
    OBJECT(ResourceEditor);

public:

    ResourceEditor(Context* context, const String& fullpath, TBTabContainer* container);

    virtual ~ResourceEditor();

    TBButton* GetButton() { return button_; }

    virtual bool HasUnsavedModifications() { return false; }

    virtual void SetFocus() {}
    virtual void Close(bool navigateToAvailabeResource = true);

    virtual bool OnEvent(const TBWidgetEvent &ev) { return false; }

    virtual bool FindText(const String& text, unsigned flags) { return false; }
    virtual void FindTextClose() { }

    virtual bool RequiresInspector() { return false; }

    const String& GetFullPath() { return fullpath_; }

    TBWidget* GetRootContentWidget() { return rootContentWidget_; }

protected:

    String fullpath_;
    TBTabContainer* container_;
    EditorTabLayout* editorTabLayout_;
    TBWidget* rootContentWidget_;
    TBButton* button_;

private:

    void HandleFileChanged(StringHash eventType, VariantMap& eventData);

};

}
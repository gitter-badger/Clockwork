#pragma once

#include <Clockwork/Core/Object.h>
#include <Clockwork/IO/FileSystem.h>

using namespace Clockwork;

namespace ClockworkEditor
{

class EditorShortcuts : public Object
{

    OBJECT(EditorShortcuts);

public:

    /// Construct.
    EditorShortcuts(Context* context);
    /// Destruct.
    ~EditorShortcuts();

    void InvokeFileClose();
    void InvokeFileSave();

    void InvokeEditCut();
    void InvokeEditCopy();
    void InvokeEditPaste();
    void InvokeEditSelectAll();

    void InvokeEditUndo();
    void InvokeEditRedo();

    void InvokeEditFind();
    void InvokeEditFindNext();
    void InvokeEditFindPrev();

    void InvokeEditFormatCode();

    void InvokeBuild();
    void InvokeBuildSettings();

    void InvokePlayStop();

private:

    bool IsProjectLoaded();

    void InvokeResourceFrameShortcut(const String& id);

    void HandleKeyDown(StringHash eventType, VariantMap& eventData);
    void HandleKeyUp(StringHash eventType, VariantMap& eventData);
    void HandleEditorShutdown(StringHash eventType, VariantMap& eventData);
};


}

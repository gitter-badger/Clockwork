#pragma once

#include <Clockwork/Core/Object.h>
#include <Clockwork/IO/FileSystem.h>

using namespace Clockwork;

#define EDITOR_STRING(x) (GetSubsystem<EditorStrings>()->Str(EditorStrings::x))

namespace ClockworkEditor
{

class EditorStrings : public Object
{

    OBJECT(EditorStrings);

public:

    enum EditorString
    {
        RevealInFinder,
        ShortcutUndo,
        ShortcutRedo,
        ShortcutCut,
        ShortcutCopy,
        ShortcutPaste,
        ShortcutSelectAll,
        ShortcutFind,
        ShortcutFindNext,
        ShortcutFindPrev,
        ShortcutBeautify,
        ShortcutCloseFile,
        ShortcutSaveFile,
        ShortcutPlay,
        ShortcutBuild,
        ShortcutBuildSettings,
    };


    /// Construct.
    EditorStrings(Context* context);
    /// Destruct.
    ~EditorStrings();

    const String& Str(EditorString estring);

private:

    void InitializeStrings();

    void HandleEditorShutdown(StringHash eventType, VariantMap& eventData);

    HashMap<unsigned, String> stringLookup_;


};


}

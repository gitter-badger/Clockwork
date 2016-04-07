#pragma once

#include <Clockwork/Core/Object.h>
#include <Clockwork/IO/File.h>

using namespace Clockwork;

namespace ClockworkEditor
{

// as resource creation gets more complicated, will want to split this up
// and delegate
class ResourceOps : public Object
{
    OBJECT(ResourceOps);

public:
    /// Construct.
    ResourceOps(Context* context);
    /// Destruct.
    virtual ~ResourceOps();

    void HandleResourceDelete(const String& resourcePath, bool reportError = true);
    void HandleNewFolder(const String& resourcePath, bool reportError = true);
    void HandleCreateComponent(const String& resourcePath, const String& resourceName, bool navigateToResource = true, bool reportError = true);
    void HandleCreateScript(const String& resourcePath, const String& resourceName, bool navigateToResource = true, bool reportError = true);
    void HandleCreateModule(const String& resourcePath, const String& resourceName, bool navigateToResource = true, bool reportError = true);
    void HandleCreate2DLevel(const String& resourcePath, const String& resourceName, bool navigateToResource = false, bool reportError = true);
private:

    void HandleEditorShutdown(StringHash eventType, VariantMap& eventData);

    bool CopyFile(File* srcFile, const String& destFileName);

    bool CheckCreateComponent(const String& resourcePath, const String& resourceName, bool reportError);
    bool CheckCreateScript(const String& resourcePath, const String& resourceName, bool reportError);
    bool CheckCreateModule(const String& resourcePath, const String& resourceName, bool reportError);
    bool CheckCreate2DLevel(const String& resourcePath, const String& resourceName, bool reportError);

};


}


#pragma once

#include <Clockwork/Scene/Component.h>

namespace Clockwork
{

class ScriptComponentFile;

class CLOCKWORK_API ScriptComponent : public Component
{
    OBJECT(ScriptComponent);

public:

    /// Construct.
    ScriptComponent(Context* context);
    /// Destruct.
    virtual ~ScriptComponent();

    static void RegisterObject(Context* context);

    virtual const String& GetComponentClassName() const = 0;

    virtual ScriptComponentFile* GetComponentFile() = 0;

    VariantMap& GetFieldValues() { return fieldValues_; }


protected:

    VariantMap fieldValues_;

};

}

#pragma once

#include <Clockwork/Core/Context.h>
#include <Clockwork/Core/Object.h>

namespace Clockwork
{

class NETCore;
class CSManaged;

class CLOCKWORK_API NETScript : public Object
{

    OBJECT(NETScript);

public:
    /// Construct.
    NETScript(Context* context);
    /// Destruct.
    virtual ~NETScript();

    bool Initialize();

    bool ExecMainAssembly();

private:

    WeakPtr<NETCore> netCore_;
    WeakPtr<CSManaged> csManaged_;

};

void RegisterNETScriptLibrary(Context* context);

}

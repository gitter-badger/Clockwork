// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#include <Clockwork/Core/Object.h>

#include "CETypes.h"
#include "Net/CurlManager.h"
#include "UI/Modal/UIProgressModal.h"

using namespace Clockwork;

namespace ClockworkEditor
{

class VersionCheck : public Object
{
    OBJECT(VersionCheck);

public:

    /// Construct.
    VersionCheck(Context* context);
    /// Destruct.
    virtual ~VersionCheck();

    void DoVersionCheck();

private:
    void HandleEditorShutdown(StringHash eventType, VariantMap& eventData);
    void HandleCurlComplete(StringHash eventType, VariantMap& eventData);

    SharedPtr<CurlRequest> versionRequest_;
    SharedPtr<ProgressModal> progressModal_;
};

}

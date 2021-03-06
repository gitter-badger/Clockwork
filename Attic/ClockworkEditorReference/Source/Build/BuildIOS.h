// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#pragma once

#include "BuildBase.h"

namespace ClockworkEditor
{

class BuildIOS : public BuildBase
{
    OBJECT(BuildIOS);

public:

    BuildIOS(Context* context);
    virtual ~BuildIOS();

    void Build(const String& buildPath);

protected:

    void Initialize();

private:

    void HandleEvent(StringHash eventType, VariantMap& eventData);

    enum BuildPhase
    {
        ConvertPList,
        CodeSign,
        Deploy
    };

    void RunConvertPList();
    void RunCodeSign();
    void RunDeploy();

    String GenerateEntitlements();
    String GenerateInfoPlist();

    BuildPhase currentBuildPhase_;

};

}

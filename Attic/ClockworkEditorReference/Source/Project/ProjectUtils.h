// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#pragma once

#include <Clockwork/Core/Object.h>

using namespace Clockwork;

namespace ClockworkEditor
{

class ProjectUtils : public Object
{
    OBJECT(ProjectUtils);

    void OpenProjectFileDialog();
    String NewProjectFileDialog();
    void RevealInFinder(const String& fullpath);

    String GetBuildPath(const String& defaultPath);
    String GetAndroidSDKPath(const String& defaultPath);
    String GetAntPath(const String& defaultPath);
    String GetJDKRootPath(const String& defaultPath);
    String GetMobileProvisionPath();

public:
    /// Construct.
    ProjectUtils(Context* context);
    /// Destruct.
    ~ProjectUtils();

private:

    void HandleEditorShutdown(StringHash eventType, VariantMap& eventData);

};


}

#pragma once

#include <Clockwork/Container/Str.h>
#include <Clockwork/Core/Object.h>

#include "BuildSettings.h"

using namespace Clockwork;

namespace ClockworkEditor
{

class AndroidProjectGenerator : public Object
{

    OBJECT(AndroidProjectGenerator);

public:

    AndroidProjectGenerator(Context* context);
    virtual ~AndroidProjectGenerator();

    void SetBuildPath(const String& buildpath) { buildPath_ = buildpath; }

    bool Generate();

    const String& GetErrorText() { return errorText_; }

private:

    bool GenerateAndroidManifest();
    bool GenerateStringXML();
    bool GenerateLocalProperties();
    bool GenerateProjectProperties();
    bool GenerateActivitySource();

    String buildPath_;
    String errorText_;
    AndroidBuildSettings buildSettings_;

};

}

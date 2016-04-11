// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#pragma once

#include <Clockwork/Engine/Application.h>

using namespace Clockwork;

namespace ClockworkEditor
{

/// ClockworkEditor application runs a script specified on the command line.
class CEApplication : public Application
{
    OBJECT(CEApplication);

public:
    /// Construct.
    CEApplication(Context* context);

    /// Setup before engine initialization. Verify that a script file has been specified.
    virtual void Setup();
    /// Setup after engine initialization. Load the script and execute its start function.
    virtual void Start();
    /// Cleanup after the main loop. Run the script's stop function if it exists.
    virtual void Stop();

private:

    String cmdLineProjectFile_;
    
};

}

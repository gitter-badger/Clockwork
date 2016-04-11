// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#pragma once

#include <Poco/Process.h>
#include <Poco/Pipe.h>
#include <Poco/PipeStream.h>

#include <Clockwork/Container/Str.h>
#include <Clockwork/Core/Mutex.h>
#include <Clockwork/Core/Thread.h>
#include <Clockwork/Core/Object.h>

using namespace Clockwork;

namespace ClockworkEditor
{

class SubprocessSystem;

class Subprocess : public Object, public Thread
{
    friend class SubprocessSystem;

    OBJECT(Subprocess);

public:
    /// Construct.
    Subprocess(Context* context);
    /// Destruct.
    ~Subprocess();

    void GetOutput(String& outputText, String& errorText)
    {
        mutex_.Acquire();
        outputText = output_;
        output_.Clear();
        errorText = errorOutput_;
        errorOutput_.Clear();
        mutex_.Release(); }

private:

    void ThreadFunction();

    mutable Mutex mutex_;

    void ProcessOutput(SubprocessSystem* system);
    bool Update(SubprocessSystem* system);


    bool Launch(const String& command, const Vector<String>& args, const String& initialDirectory = "");
    bool Launch(const String& command, const Vector<String>& args, const String& initialDirectory, const Poco::Process::Env& env);

    String output_;
    String errorOutput_;
    int returnCode_;

    Poco::ProcessHandle* processHandle_;
    Poco::Pipe pipeIn_;
    Poco::Pipe pipeOut_;
    Poco::Pipe pipeError_;
    Poco::PipeInputStream inStream_;
    Poco::PipeInputStream errorStream_;
};

}

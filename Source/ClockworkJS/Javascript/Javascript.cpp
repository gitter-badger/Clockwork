//
// Copyright (c) 2014-2015, CLOCKWORK STUDIOS All rights reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include <Clockwork/Core/CoreEvents.h>
#include <Clockwork/Engine/EngineEvents.h>
#include <Clockwork/IO/File.h>
#include <Clockwork/IO/Log.h>
#include <Clockwork/Core/ProcessUtils.h>
#include <Clockwork/Core/Profiler.h>
#include <Clockwork/Resource/ResourceCache.h>
#include <Clockwork/Scene/Scene.h>

#include "Javascript.h"
#include "JSComponent.h"
#include "JSComponentFile.h"
#include "JSVM.h"

namespace Clockwork
{

Javascript::Javascript(Context* context) :
    Object(context)
{
    RegisterJavascriptLibrary(context_);
}

Javascript::~Javascript()
{

}

JSVM* Javascript::InstantiateVM(const String& name)
{
    if (vms_.Contains(name))
        return NULL;

    JSVM* vm = new JSVM(context_);
    vms_[name] = vm;
    return vm;

}

void Javascript::ShutdownVM(const String& name)
{
    vms_.Erase(name);
}

void RegisterJavascriptLibrary(Context* context)
{
    JSComponentFile::RegisterObject(context);
    JSComponent::RegisterObject(context);
}

}

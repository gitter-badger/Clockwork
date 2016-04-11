//
// Copyright (c) 2014-2016 THUNDERBEAST GAMES LLC
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

#pragma once

#include <Clockwork/Core/Object.h>

using namespace Clockwork;

namespace ToolCore
{

class BuildBase;
class Project;

enum PlatformID
{
    PLATFORMID_UNDEFINED,
    PLATFORMID_WINDOWS,
    PLATFORMID_MAC,
    PLATFORMID_ANDROID,
    PLATFORMID_IOS,
    PLATFORMID_WEB
};

class Platform : public Object
{
    OBJECT(Platform);

public:

    Platform(Context* context);
    virtual ~Platform();

    virtual bool GetLicense() = 0;

    virtual String GetName() = 0;
    virtual PlatformID GetPlatformID() = 0;

    virtual BuildBase* NewBuild(Project* project) = 0;

private:

    bool validLicense_;

};

}
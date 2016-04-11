//
// Copyright (c) 2014-2016 CLOCKWORK STUDIOS
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

#include <Clockwork/Engine/Application.h>
#include <Clockwork/Resource/JSONFile.h>

using namespace Clockwork;

namespace Clockwork
{
    class JSVM;
}

namespace ClockworkEditor
{

class CEEditorCommon : public Application
{
    OBJECT(CEEditorCommon);

public:
    /// Construct.
    CEEditorCommon(Context* context);

    /// Setup before engine initialization. Verify that a script file has been specified.
    virtual void Setup();
    /// Setup after engine initialization. Load the script and execute its start function.
    virtual void Start();
    /// Cleanup after the main loop. Run the script's stop function if it exists.
    virtual void Stop();

protected:

    bool CreateDefaultPreferences(String& path, JSONValue& prefs);
    bool ReadPreferences();
    void ValidateWindow();

    SharedPtr<JSVM> vm_;

private:
    void GetDefaultWindowPreferences(JSONValue& windowPrefs, bool maximized);
    String GetPreferencesPath();
    bool LoadPreferences(JSONValue& prefs);
    bool SavePreferences(JSONValue& prefs);
};

}

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

#pragma once

#include <Clockwork/Resource/Resource.h>
#include <Clockwork/Resource/JSONValue.h>
#include <Clockwork/Container/ArrayPtr.h>
#include <Clockwork/Container/List.h>

#include <Clockwork/Script/ScriptComponentFile.h>

namespace Clockwork
{

class CSComponent;
class NETVariantMap;

// At runtime we need to load the assembly, in the editor we use metadata
/// NET Assembly resource.
class CLOCKWORK_API CSComponentAssembly : public ScriptComponentFile
{
    OBJECT(CSComponentAssembly);
    BASEOBJECT(CSComponentAssembly);

public:

    /// Construct.
    CSComponentAssembly(Context* context);
    /// Destruct.
    virtual ~CSComponentAssembly();

    bool ParseAssemblyJSON(const JSONValue& json);

    CSComponent* CreateCSComponent(const String& classname);

    /// Load resource from stream. May be called from a worker thread. Return true if successful.
    virtual bool BeginLoad(Deserializer& source);
    /// Save resource
    virtual bool Save(Serializer& dest) const;

    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Only valid in editor, as we don't inspect assembly at runtime
    const Vector<String>& GetClassNames() { return classNames_; }

private:

    static void InitTypeMap();

    bool ParseComponentClassJSON(const JSONValue& json);

    // only valid in editor
    Vector<String> classNames_;

    HashMap<String, Vector<EnumInfo>> assemblyEnums_;
    static HashMap<StringHash, VariantType> typeMap_;

};

}
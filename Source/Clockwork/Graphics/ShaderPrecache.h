//
// Copyright (c) 2008-2015 the Clockwork project.
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

#include "../Container/HashSet.h"
#include "../Core/Object.h"
#include "../Resource/XMLFile.h"

namespace Clockwork
{

class Graphics;
class ShaderVariation;

struct ShaderCombinationRecord
{
    ShaderVariation* vs_;
    ShaderVariation* ps_;
    ShaderVariation* gs_;

    ShaderCombinationRecord() :
        vs_(0), ps_(0), gs_(0)
    {

    }

    ShaderCombinationRecord(const ShaderCombinationRecord& rhs) :
        vs_(rhs.vs_), ps_(rhs.ps_), gs_(rhs.gs_)
    {

    }

    ShaderCombinationRecord(ShaderVariation* vs, ShaderVariation* ps, ShaderVariation* gs) :
        vs_(vs), ps_(ps), gs_(gs)
    {

    }

    bool operator<(const ShaderCombinationRecord& rhs) const
    {
        return vs_ < rhs.vs_ && ps_ < rhs.ps_ && gs_ < rhs.gs_;
    }

    bool operator>(const ShaderCombinationRecord& rhs) const
    {
        return vs_ > rhs.vs_ && ps_ > rhs.ps_ && gs_ > rhs.gs_;
    }

    bool operator==(const ShaderCombinationRecord& rhs) const
    {
        return vs_ == rhs.vs_ && ps_ == rhs.ps_ && gs_ == rhs.gs_;
    }

    unsigned ToHash() const
    {
        return MakeHash(vs_) + (MakeHash(ps_) << 16) + MakeHash(gs_);
    }
};


/// Utility class for collecting used shader combinations during runtime for precaching.
class CLOCKWORK_API ShaderPrecache : public Object
{
    OBJECT(ShaderPrecache);

public:
    /// Construct and begin collecting shader combinations. Load existing combinations from XML if the file exists.
    ShaderPrecache(Context* context, const String& fileName);
    /// Destruct. Write the collected shaders to XML.
    ~ShaderPrecache();

    /// Collect a shader combination. Called by Graphics when shaders have been set.
    void StoreShaders(ShaderVariation* vs, ShaderVariation* ps, ShaderVariation* gs);

    /// Load shaders from an XML file.
    static void LoadShaders(Graphics* graphics, Deserializer& source);

private:
    /// XML file name.
    String fileName_;
    /// XML file.
    XMLFile xmlFile_;
    /// Already encountered shader combinations, pointer version for fast queries.
    HashSet<ShaderCombinationRecord> usedPtrCombinations_; 
    /// Already encountered shader combinations.
    HashSet<String> usedCombinations_;
};

}

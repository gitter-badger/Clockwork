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

#include "../IO/Deserializer.h"
#include "../Graphics/GPUObject.h"
#include "../Core/Object.h"
#include "../Core/Object.h"

namespace Clockwork
{
enum ComputeBufferType
{
    CBT_SRV,
    CBT_UNORDERED_ACCESS_VIEW
};

enum ComputeBufferUsage
{
    CBU_DEFAULT = 0,
    CBU_IMMUTABLE,
    CBU_DYNAMIC
};

class ComputeBuffer : public Object, public GPUObject, public Serializer, public Deserializer
{
    OBJECT(ComputeBuffer);
public:
    /// Construct.
    ComputeBuffer(Context* context);
    /// Destruct.
    virtual ~ComputeBuffer();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Initialize the GPU buffer
    void CreateBuffer(ComputeBufferType type, ComputeBufferUsage usage, unsigned size);
    /// Release the GPU resources.
    void Release();

    /// Get the type of the GPU buffer.
    ComputeBufferType GetBufferType() const { return type_; }

private:
    ComputeBufferType type_;
    ComputeBufferUsage usage_;
    void* bufferObject_;
};

}  


}

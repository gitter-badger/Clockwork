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

#include "IO/Deserializer.h"  
#include "Graphics/GPUObject.h"
#include "Resource/Resource.h"
#include "IO/Serializer.h"

namespace Clockwork
{

class GPUBuffer : public Object, public GPUObject, public Serializer, public Deserializer
{
    OBJECT(GPUBuffer);
public:
    /// Construct.
    GPUBuffer(Context* context);
    /// Destruct.
    virtual ~GPUBuffer();

    void CreateBuffer(GPUBufferType type, unsigned size, unsigned stride);
    virtual void Release();
    void SetData(void* data);

    /// Read bytes from the buffer. Return number of bytes actually read.
    virtual unsigned Read(void* dest, unsigned size);
    /// Set position from the beginning of the stream.
    virtual unsigned Seek(unsigned position);
    /// Write bytes to the buffer. Return number of bytes actually written.
    virtual unsigned Write(const void* data, unsigned size);

private:
    PODVector<unsigned char> data_;
    /// Position for writing and reading.
    unsigned caretPos_;

    /// Shader resource view if available.
    void* srv_;
    /// Unordered access view if available.
    void* uav_;
};

}

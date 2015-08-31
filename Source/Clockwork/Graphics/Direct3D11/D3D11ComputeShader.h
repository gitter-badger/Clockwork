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

#include "../Core/Object.h"
#include "../Graphics/GraphicsDefs.h"
#include "../Graphics/Shader.h"

namespace Clockwork
{

class ComputeBuffer;

class CLOCKWORK_API ComputeShader : public Shader
{
    OBJECT(ComputeShader);
public:
    /// Construct.
    ComputeShader(Context* context);
    /// Destruct.
    virtual ~ComputeShader();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Dispatches the compute shader.
    void Execute(ComputeBuffer* computeBuffer);

    /// Returns the number of X axis threads.
    unsigned GetThreadsX() const { return threadsX_; }
    /// Returns the number of Y axis threads.
    unsigned GetThreadsY() const { return threadsY_; }
    /// Returns the number of Z axis threads.
    unsigned GetThreadsZ() const { return threadsZ_; }

    /// Returns the number of X axis threads.
    void SetThreadsX(unsigned threadCount) { threadsX_ = threadCount; }
    /// Returns the number of Y axis threads.
    void SetThreadsY(unsigned threadCount) { threadsY_ = threadCount; }
    /// Returns the number of Z axis threads.
    void SetThreadsZ(unsigned threadCount) { threadsZ_ = threadCount; }

private:
    /// Number of X axis threads.
    unsigned threadsX_;
    /// Number of Y axis threads.
    unsigned threadsY_;
    /// Number of Z axis threads.
    unsigned threadsZ_;
};

}

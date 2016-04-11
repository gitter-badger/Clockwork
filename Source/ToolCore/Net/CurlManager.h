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

typedef void CURL;

#include <Clockwork/Core/Object.h>
#include <Clockwork/Container/Str.h>
#include <Clockwork/Core/Mutex.h>
#include <Clockwork/Core/Thread.h>

using namespace Clockwork;

namespace ToolCore
{

EVENT(E_CURLCOMPLETE, CurlComplete)
{
    PARAM(P_CURLREQUEST, Request);      // CurlRequest*
}

class CurlRequest : public Object, public Thread
{
    friend class CurlManager;

    OBJECT(CurlRequest);

public:

    CurlRequest(Context* context, const String& url, const String& postData = String::EMPTY);
    virtual ~CurlRequest();

    const String& GetError() { return error_; }
    const String& GetResponse() { return response_; }

private:

    void ThreadFunction();
    mutable Mutex mutex_;

    static size_t Writer(void *ptr, size_t size, size_t nmemb, void *userp);

    CURL* curl_;
    String response_;
    String error_;

    String postData_;
    String url_;
};

class CurlManager : public Object
{
    OBJECT(CurlManager);

public:
    /// Construct.
    CurlManager(Context* context);
    /// Destruct.
    virtual ~CurlManager();

    SharedPtr<CurlRequest> MakeRequest(const String& url, const String& postData = String::EMPTY);

private:

    void HandleUpdate(StringHash eventType, VariantMap& eventData);

    Vector< SharedPtr<CurlRequest> > requests_;

};

}

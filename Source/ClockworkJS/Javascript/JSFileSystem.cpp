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

#include <Clockwork/Core/ProcessUtils.h>

#include "JSCore.h"
#include "JSEventHelper.h"
#include "JSVM.h"

namespace Clockwork
{

static int Clockwork_SplitPath(duk_context* ctx)
{
    String path = duk_require_string(ctx, 0);

    String pathName;
    String fileName;
    String ext;

    duk_push_object(ctx);

    SplitPath(path, pathName, fileName, ext);

    duk_push_string(ctx, pathName.CString());
    duk_put_prop_string(ctx, -2, "pathName");

    duk_push_string(ctx, fileName.CString());
    duk_put_prop_string(ctx, -2, "fileName");

    duk_push_string(ctx, ext.CString());
    duk_put_prop_string(ctx, -2, "ext");

    return 1;

}

static int Clockwork_AddTrailingSlash(duk_context* ctx)
{
    String path = duk_require_string(ctx, 0);

    duk_push_string(ctx, AddTrailingSlash(path).CString());

    return 1;
}

static int Clockwork_GetExtenstion(duk_context* ctx)
{
    String path = duk_require_string(ctx, 0);

    duk_push_string(ctx, GetExtension(path).CString());

    return 1;
}

static int Clockwork_GetParentPath(duk_context* ctx)
{
    String path = duk_require_string(ctx, 0);

    duk_push_string(ctx, GetParentPath(path).CString());

    return 1;
}

static int FileSystem_ScanDir(duk_context* ctx)
{
    duk_push_this(ctx);

    FileSystem* fs = js_to_class_instance<FileSystem>(ctx, -1, 0);

    if ( !duk_is_string(ctx, 0) || !duk_is_string(ctx, 1) ||
            !duk_is_number(ctx, 2) || !duk_is_boolean(ctx, 3))
    {
        duk_push_string(ctx, "FileSystem::ScanDir bad args");
        duk_throw(ctx);
    }

    const char* pathName = duk_to_string(ctx, 0);
    const char* filter = duk_to_string(ctx, 1);
    unsigned flags = duk_to_number(ctx, 2);
    bool recursive = duk_to_boolean(ctx, 3) ? true : false;

    Vector<String> result;

    fs->ScanDir(result, pathName, filter, flags, recursive);

    duk_push_array(ctx);

    for (unsigned i = 0; i < result.Size(); i++)
    {
        duk_push_string(ctx, result[i].CString());
        duk_put_prop_index(ctx, -2, i);
    }

    return 1;
}

void jsapi_init_filesystem(JSVM* vm)
{
    duk_context* ctx = vm->GetJSContext();

    duk_get_global_string(ctx, "Clockwork");

    duk_push_c_function(ctx, Clockwork_SplitPath, 1);
    duk_put_prop_string(ctx, -2, "splitPath");

    duk_push_c_function(ctx, Clockwork_AddTrailingSlash, 1);
    duk_put_prop_string(ctx, -2, "addTrailingSlash");

    duk_push_c_function(ctx, Clockwork_GetParentPath, 1);
    duk_put_prop_string(ctx, -2, "getParentPath");

    duk_push_c_function(ctx, Clockwork_GetExtenstion, 1);
    duk_put_prop_string(ctx, -2, "getExtension");

    duk_pop(ctx); // pop Clockwork object

    js_class_get_prototype(ctx, "Clockwork", "FileSystem");
    duk_push_c_function(ctx, FileSystem_ScanDir, 4);
    duk_put_prop_string(ctx, -2, "scanDir");
    duk_pop(ctx);

}

}

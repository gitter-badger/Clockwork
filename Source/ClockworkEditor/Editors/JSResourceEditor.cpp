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

#include <Clockwork/Container/ArrayPtr.h>
#include <Clockwork/UI/UI.h>
#include <Clockwork/IO/Log.h>
#include <Clockwork/IO/File.h>
#include <Clockwork/IO/FileSystem.h>
#include <Clockwork/Resource/ResourceCache.h>
#include <Clockwork/Resource/JSONFile.h>

#include <Clockwork/Core/CoreEvents.h>
#include <ClockworkJS/Javascript/JSVM.h>

#include <ToolCore/ToolEnvironment.h>

#include <ClockworkWebView/WebViewEvents.h>
#include <ClockworkWebView/UIWebView.h>
#include <ClockworkWebView/WebClient.h>
#include <ClockworkWebView/WebMessageHandler.h>
#include <ClockworkWebView/WebTexture2D.h>

#include "JSResourceEditor.h"


using namespace tb;
using namespace ToolCore;

namespace ClockworkEditor
{

JSResourceEditor ::JSResourceEditor(Context* context, const String &fullpath, UITabContainer *container) :
    ResourceEditor(context, fullpath, container)
{

    TBLayout* layout = new TBLayout();
    layout->SetLayoutSize(LAYOUT_SIZE_GRAVITY);
    layout->SetGravity(WIDGET_GRAVITY_ALL);
    layout->SetLayoutDistribution(LAYOUT_DISTRIBUTION_GRAVITY);

    rootContentWidget_->GetInternalWidget()->AddChild(layout);

    TBContainer* c = new TBContainer();
    c->SetGravity(WIDGET_GRAVITY_ALL);

    layout->AddChild(c);

    ToolEnvironment* tenv = GetSubsystem<ToolEnvironment>();
    String codeEditorDir = tenv->GetToolDataDir();
    codeEditorDir += "CodeEditor/Editor.html";

#ifdef CLOCKWORK_PLATFORM_OSX
    String url = "file://" + codeEditorDir;
#else
    String url = "file:///" + codeEditorDir;
#endif

    webView_ = new UIWebView(context_, url);
    webClient_ = webView_->GetWebClient();
    messageHandler_ = new WebMessageHandler(context_);
    webClient_->AddMessageHandler(messageHandler_);

    webView_->GetWebTexture2D()->SetClearColor(Color(.23f, .23f, .23f, 1));

    SubscribeToEvent(webClient_, E_WEBVIEWLOADEND, HANDLER(JSResourceEditor, HandleWebViewLoadEnd));
    SubscribeToEvent(messageHandler_, E_WEBMESSAGE, HANDLER(JSResourceEditor, HandleWebMessage));


    c->AddChild(webView_->GetInternalWidget());

}

JSResourceEditor::~JSResourceEditor()
{

}

void JSResourceEditor::HandleWebViewLoadEnd(StringHash eventType, VariantMap& eventData)
{
    webClient_->ExecuteJavaScript(ToString("loadCode(\"clockwork://resources/%s\");", fullpath_.CString()));
}

void JSResourceEditor::HandleWebMessage(StringHash eventType, VariantMap& eventData)
{
    using namespace WebMessage;

    const String& request = eventData[P_REQUEST].GetString();
    WebMessageHandler* handler = static_cast<WebMessageHandler*>(eventData[P_HANDLER].GetPtr());

    if (request == "change")
    {
        SetModified(true);
    }
    else
    {
        JSONValue jvalue;
        if (JSONFile::ParseJSON(request, jvalue, false))
        {
            String message = jvalue["message"].GetString();
            if (message == "saveCode")
            {
                String code = jvalue["payload"].GetString();
                File file(context_, fullpath_, FILE_WRITE);
                file.Write((void*) code.CString(), code.Length());
                file.Close();
            }
        }
    }

    handler->Success();

}

void JSResourceEditor::FormatCode()
{
    //webClient_->ExecuteJavaScript("beautifyCode();");
}

bool JSResourceEditor::OnEvent(const TBWidgetEvent &ev)
{
    if (ev.type == EVENT_TYPE_SHORTCUT)
    {
        if (ev.ref_id == TBIDC("close"))
        {
            RequestClose();
        }
    }

    return false;
}

void JSResourceEditor::FindTextClose()
{
}

bool JSResourceEditor::FindText(const String& findText, unsigned flags)
{

    return true;
}

void JSResourceEditor::SetFocus()
{
    //editField_->SetFocus(WIDGET_FOCUS_REASON_UNKNOWN);
}

void JSResourceEditor::GotoTokenPos(int tokenPos)
{

}

void JSResourceEditor::GotoLineNumber(int lineNumber)
{

}

bool JSResourceEditor::Save()
{
    if (!modified_)
        return true;

    webClient_->ExecuteJavaScript("saveCode();");

    SetModified(false);

    return true;

}


}

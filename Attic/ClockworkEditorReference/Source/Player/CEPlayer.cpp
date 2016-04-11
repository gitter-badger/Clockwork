// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/ClockworkGameEngine/ClockworkGameEngine

#include "ClockworkEditor.h"

#include "../Subprocess/CESubprocessSystem.h"

#include <Clockwork/Core/Context.h>
#include <Clockwork/Core/StringUtils.h>
#include <Clockwork/IO/FileSystem.h>
#include <Clockwork/IO/Log.h>
#include <Clockwork/Input/Input.h>
#include <Clockwork/Resource/ResourceCache.h>
#include <Clockwork/UI/UI.h>

#include <ToolCore/ToolEnvironment.h>

#include "CEPlayer.h"
#include "CEEvents.h"
#include "CEEditor.h"
#include "Project/CEProject.h"

#include "UIPlayer.h"
#include "UI/Modal/UIModalOps.h"

namespace ClockworkEditor
{


CEPlayer::CEPlayer(Context* context) :
    Object(context),
    mode_(CE_PLAYERMODE_WIDGET)
{
    SubscribeToEvent(E_EDITORSHUTDOWN, HANDLER(CEPlayer, HandleEditorShutdown));
    SubscribeToEvent(E_IPCWORKERSTART, HANDLER(CEPlayer, HandleIPCWorkerStarted));

    assert(!context->GetSubsystem<CEPlayer>());
    context->RegisterSubsystem(this);

}

CEPlayer::~CEPlayer()
{

}

void CEPlayer::Invalidate()
{
    UIModalOps* ops = GetSubsystem<UIModalOps>();
    ops->Hide();
    context_->RemoveSubsystem<CEPlayer>();
}

void CEPlayer::HandleJSError(StringHash eventType, VariantMap& eventData)
{

}

void CEPlayer::HandleIPCWorkerStarted(StringHash eventType, VariantMap& eventData)
{
    //VariantMap weventData;
    //broker_->PostMessage(E_IPCSTARTUP, weventData);
}

void CEPlayer::HandleIPCWorkerExit(StringHash eventType, VariantMap& eventData)
{
    SendEvent(E_EDITORPLAYSTOP);
}


bool CEPlayer::Play(CEPlayerMode mode, const IntRect &rect)
{
    Editor* editor = GetSubsystem<Editor>();
    ToolCore::ToolEnvironment* env = GetSubsystem<ToolCore::ToolEnvironment>();
    const String& editorBinary = env->GetEditorBinary();

    Project* project = editor->GetProject();

    Vector<String> paths;
    paths.Push(env->GetCoreDataDir());
    paths.Push(env->GetPlayerDataDir());
    paths.Push(project->GetResourcePath());

    String resourcePaths;
    resourcePaths.Join(paths, "!");

    Vector<String> vargs;

    String args = ToString("--editor-resource-paths \"%s\"", resourcePaths.CString());

    vargs = args.Split(' ');
    vargs.Insert(0, "--player");

    String dump;
    dump.Join(vargs, " ");
    LOGINFOF("Launching Broker %s %s", editorBinary.CString(), dump.CString());

    IPC* ipc = GetSubsystem<IPC>();
    broker_ = ipc->SpawnWorker(editorBinary, vargs);

    if (broker_)
    {
        SubscribeToEvent(broker_, E_IPCWORKEREXIT, HANDLER(CEPlayer, HandleIPCWorkerExit));
    }

    return broker_.NotNull();
}

void CEPlayer::SetUIPlayer(UIPlayer* uiPlayer)
{
    uiPlayer_ = uiPlayer;
}

void CEPlayer::HandleEditorShutdown(StringHash eventType, VariantMap& eventData)
{
    context_->RemoveSubsystem(GetType());
}

}



#include "../Precompiled.h"

#include "../LuaScript/LuaFunction.h"
#include "../LuaScript/LuaScriptEventInvoker.h"
#include "../LuaScript/LuaScriptInstance.h"

#include "../DebugNew.h"

namespace Clockwork
{

LuaScriptEventInvoker::LuaScriptEventInvoker(Context* context) :
    Object(context)
{
}

LuaScriptEventInvoker::LuaScriptEventInvoker(LuaScriptInstance* instance) :
    Object(instance->GetContext()),
    instance_(instance)
{
}

LuaScriptEventInvoker::~LuaScriptEventInvoker()
{
}

void LuaScriptEventInvoker::AddEventHandler(Object* sender, const StringHash& eventType, LuaFunction* function)
{
    if (!function)
        return;

    if (sender)
        SubscribeToEvent(sender, eventType, HANDLER_USERDATA(LuaScriptEventInvoker, HandleLuaScriptEvent, function));
    else
        SubscribeToEvent(eventType, HANDLER_USERDATA(LuaScriptEventInvoker, HandleLuaScriptEvent, function));
}

void LuaScriptEventInvoker::HandleLuaScriptEvent(StringHash eventType, VariantMap& eventData)
{
    LuaFunction* function = static_cast<LuaFunction*>(GetEventHandler()->GetUserData());
    if (!function)
        return;

    // Keep instance alive during invoking
    SharedPtr<LuaScriptInstance> instance(instance_);
    if (instance)
    {
        if (function->BeginCall(instance))
        {
            function->PushUserType(eventType, "StringHash");
            function->PushUserType(eventData, "VariantMap");
            function->EndCall();
        }
    }
    else
    {
        if (function->BeginCall())
        {
            function->PushUserType(eventType, "StringHash");
            function->PushUserType(eventData, "VariantMap");
            function->EndCall();
        }
    }
}

}

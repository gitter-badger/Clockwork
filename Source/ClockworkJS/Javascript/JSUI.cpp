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

#include <TurboBadger/tb_widgets.h>

#include <Clockwork/Core/CoreEvents.h>
#include <Clockwork/UI/UIEvents.h>
#include <Clockwork/UI/UIWidget.h>
#include <Clockwork/UI/UIButton.h>
#include <Clockwork/UI/UI.h>
#include "JSVM.h"
#include "JSUI.h"

using namespace tb;

namespace Clockwork
{

JSUI::JSUI(Context* context) : Object(context),
    updateTime_(0.0f)
{
    ctx_ = JSVM::GetJSVM(nullptr)->GetJSContext();

    SubscribeToEvent(E_UPDATE, HANDLER(JSUI, HandleUpdate));

    SubscribeToEvent(E_JSOBJECTADDED, HANDLER(JSUI, HandleObjectAdded));

    // for debugging only, commented out otherwise
    //SubscribeToEvent(E_JSOBJECTREMOVED, HANDLER(JSUI, HandleObjectRemoved));

    SubscribeToEvent(E_WIDGETDELETED, HANDLER(JSUI, HandleWidgetDeleted));
    SubscribeToEvent(E_WIDGETEVENT, HANDLER(JSUI, HandleWidgetEvent));
    SubscribeToEvent(E_WIDGETLOADED, HANDLER(JSUI, HandleWidgetLoaded));
    SubscribeToEvent(E_POPUPMENUSELECT, HANDLER(JSUI, HandlePopupMenuSelect));

    duk_push_global_stash(ctx_);
    duk_push_object(ctx_);
    duk_put_prop_string(ctx_, -2, "__jsui_widgetkeepalive");
    duk_pop(ctx_);

    uiTypes_["UIWidget"] = true;
    uiTypes_["UIButton"] = true;
    uiTypes_["UIView"] = true;
    uiTypes_["UIEditField"] = true;
    uiTypes_["UITextField"] = true;
    uiTypes_["UIImageWidget"] = true;
    uiTypes_["UILayout"] = true;
    uiTypes_["UIMenuWindow"] = true;
    uiTypes_["UIWindow"] = true;
    uiTypes_["UIClickLabel"] = true;
    uiTypes_["UICheckBox"] = true;
    uiTypes_["UISelectLost"] = true;
    uiTypes_["UIListView"] = true;
    uiTypes_["UIMessageWindow"] = true;
    uiTypes_["UISkinImage"] = true;
    uiTypes_["UITabContainer"] = true;
    uiTypes_["UISceneView"] = true;
    uiTypes_["UIContainer"] = true;
    uiTypes_["UISection"] = true;
    uiTypes_["UIInlineSelect"] = true;
    uiTypes_["UITextureWidget"] = true;
    uiTypes_["UIScrollContainer"] = true;
    uiTypes_["UISeparator"] = true;
    uiTypes_["UIDimmer"] = true;
    uiTypes_["UISelectDropdown"] = true;
    uiTypes_["UIPopupWindow"] = true;

}

JSUI::~JSUI()
{

}

void JSUI::GatherWidgets(tb::TBWidget* widget, PODVector<tb::TBWidget*>& widgets)
{
    if (widget->GetID() != TBID())
        widgets.Push(widget);

    for (TBWidget *n = widget->GetFirstChild(); n; n = n->GetNext())
    {
        GatherWidgets(n, widgets);
    }

}

void JSUI::HandleObjectAdded(StringHash eventType, VariantMap& eventData)
{
    RefCounted* ref = static_cast<RefCounted*>(eventData[ObjectAdded::P_OBJECT].GetPtr());

    assert(ref->IsObject());

    Object* o = (Object*) ref;

    // for any UI type, we make sure it is kept alive
    if (uiTypes_.Contains(o->GetType()))
    {
        assert(o->JSGetHeapPtr());

        duk_push_global_stash(ctx_);
        duk_get_prop_string(ctx_, -1, "__jsui_widgetkeepalive");
        // can't use instance as key, as this coerces to [Object] for
        // string property, pointer will be string representation of
        // address, so, unique key
        duk_push_pointer(ctx_, o);
        duk_push_heapptr(ctx_, o->JSGetHeapPtr());
        duk_put_prop(ctx_, -3);
        duk_pop_2(ctx_);

    }

}

void JSUI::HandleObjectRemoved(StringHash eventType, VariantMap& eventData)
{
    Object* o = static_cast<Object*>(eventData[ObjectAdded::P_OBJECT].GetPtr());

    LOGINFOF("Removing %s", o->GetTypeName().CString());

}

void JSUI::HandleWidgetDeleted(StringHash eventType, VariantMap& eventData)
{
    UIWidget* widget = static_cast<UIWidget*>(eventData[WidgetDeleted::P_WIDGET].GetPtr());

    if (!widget->JSGetHeapPtr())
        return;

    duk_push_global_stash(ctx_);
    duk_get_prop_string(ctx_, -1, "__jsui_widgetkeepalive");
    // can't use instance as key, as this coerces to [Object] for
    // string property, pointer will be string representation of
    // address, so, unique key
    duk_push_pointer(ctx_, widget);
    duk_del_prop(ctx_, -2);
    duk_pop_2(ctx_);

}

void JSUI::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    float timeStep = eventData[Update::P_TIMESTEP].GetFloat();

    updateTime_ += timeStep;
    if (updateTime_ > 1.0f)
    {
        UI* ui = GetSubsystem<UI>();
        ui->PruneUnreachableWidgets();
        updateTime_ = 0.0f;
    }
}


void JSUI::HandleWidgetLoaded(StringHash eventType, VariantMap& eventData)
{
    using namespace WidgetLoaded;

    UIWidget* widget = static_cast<UIWidget*>(eventData[P_WIDGET].GetPtr());
    if (!widget)
        return;

    void* heapptr = widget->JSGetHeapPtr();

    if (!heapptr)
        return;

    TBWidget* tbwidget = widget->GetInternalWidget();
    assert(tbwidget);

    // all widgets with id's are wrapped, so that event handlers are bubbled up properly
    // note that all js widget object representations are kept alive in HandleObjectAdded
    // when pushed into the VM

    PODVector<TBWidget*> widgets;
    GatherWidgets(tbwidget, widgets);

    UI* ui = GetSubsystem<UI>();

    for (unsigned i = 0; i < widgets.Size(); i++)
    {
        UIWidget* o =  ui->WrapWidget(widgets.At(i));

        if (!o)
            continue;

        js_push_class_object_instance(ctx_, o);
    }

}

void JSUI::HandlePopupMenuSelect(StringHash eventType, VariantMap& eventData)
{
    using namespace PopupMenuSelect;

    UIButton* button = static_cast<UIButton*>(eventData[P_BUTTON].GetPtr());
    if (!button)
        return;

    void* heapptr = button->JSGetHeapPtr();

    if (!heapptr) // gc'd
        return;

    int top = duk_get_top(ctx_);

    duk_push_heapptr(ctx_, heapptr);
    duk_get_prop_string(ctx_, -1, "__popup_menu_callback");
    if (!duk_is_callable(ctx_, -1))
    {
        duk_pop_n(ctx_, 2);
        assert(top == duk_get_top(ctx_));
        return;
    }

    String id;
    id = eventData[P_REFID].GetString();

    duk_push_string(ctx_, id.CString());
    duk_insert(ctx_, -1);

    if (duk_pcall(ctx_, 1) != 0)
    {
        JSVM::GetJSVM(nullptr)->SendJSErrorEvent();
    }

    duk_pop_n(ctx_, 2);

    assert(top == duk_get_top(ctx_));

}

void JSUI::PushWidgetEventObject(VariantMap& eventData)
{

    using namespace WidgetEvent;

    // create the event object
    duk_push_object(ctx_);

    // target
    UIWidget* target = static_cast<UIWidget*>(eventData[P_TARGET].GetPtr());

    if (target)
    {
        assert(target->JSGetHeapPtr());
        duk_push_heapptr(ctx_, target->JSGetHeapPtr());
    }
    else
    {
        duk_push_null(ctx_);
    }

    duk_put_prop_string(ctx_, -2, "target");

    duk_push_number(ctx_, (duk_double_t) eventData[P_TYPE].GetUInt());
    duk_put_prop_string(ctx_, -2, "type");

    duk_push_number(ctx_, (duk_double_t) eventData[P_X].GetInt());
    duk_put_prop_string(ctx_, -2, "x");

    duk_push_number(ctx_, (duk_double_t) eventData[P_Y].GetInt());
    duk_put_prop_string(ctx_, -2, "y");

    duk_push_number(ctx_, (duk_double_t) eventData[P_DELTAX].GetInt());
    duk_put_prop_string(ctx_, -2, "deltaX");

    duk_push_number(ctx_, (duk_double_t) eventData[P_DELTAX].GetInt());
    duk_put_prop_string(ctx_, -2, "deltaY");

    duk_push_number(ctx_, (duk_double_t) eventData[P_COUNT].GetInt());
    duk_put_prop_string(ctx_, -2, "count");

    duk_push_number(ctx_, (duk_double_t) eventData[P_KEY].GetInt());
    duk_put_prop_string(ctx_, -2, "key");

    duk_push_number(ctx_, (duk_double_t) eventData[P_SPECIALKEY].GetInt());
    duk_put_prop_string(ctx_, -2, "specialKey");

    duk_push_number(ctx_, (duk_double_t) eventData[P_MODIFIERKEYS].GetInt());
    duk_put_prop_string(ctx_, -2, "modifierKeys");

    duk_push_number(ctx_, (duk_double_t) eventData[P_MODIFIERKEYS].GetInt());
    duk_put_prop_string(ctx_, -2, "modifierKeys");

    String id = eventData[P_REFID].GetString();
    duk_push_string(ctx_, id.CString() );
    duk_put_prop_string(ctx_, -2, "refID");

    duk_push_boolean(ctx_,eventData[P_TOUCH].GetBool() ? 1 : 0);
    duk_put_prop_string(ctx_, -2, "touch");

}

void JSUI::HandleWidgetEvent(StringHash eventType, VariantMap& eventData)
{
    using namespace WidgetEvent;

    UIWidget* handler = static_cast<UIWidget*>(eventData[P_HANDLER].GetPtr());
    UIWidget* target = static_cast<UIWidget*>(eventData[P_TARGET].GetPtr());

    if (!handler)
        return;

    void* handlerHeapPtr = handler->JSGetHeapPtr();

    if (!handlerHeapPtr)
        return;

    // if we have a target, however no corresponding JS object return
    if (target && !target->JSGetHeapPtr())
        return;

    tb::EVENT_TYPE type = (tb::EVENT_TYPE) eventData[P_TYPE].GetUInt();

    if (type == tb::EVENT_TYPE_CHANGED)
    {
        int top = duk_get_top(ctx_);
        duk_push_heapptr(ctx_, handlerHeapPtr);
        duk_get_prop_string(ctx_, -1, "onChanged");
        if (duk_is_callable(ctx_, -1)) {

            if (duk_pcall(ctx_, 0) != 0)
            {
                JSVM::GetJSVM(nullptr)->SendJSErrorEvent();
            }
            else
            {
                if (duk_is_boolean(ctx_, -1) && duk_to_boolean(ctx_, -1))
                    eventData[P_HANDLED] = true;
            }
        }
        duk_pop_n(ctx_, 2);
        assert(top == duk_get_top(ctx_));
        return;

    }

    // general event handler, bubbles to (wrapped) parent widgets unless handled (returns true)
    if (type == tb::EVENT_TYPE_CLICK)
    {
        int top = duk_get_top(ctx_);
        duk_push_heapptr(ctx_, handlerHeapPtr);
        duk_get_prop_string(ctx_, -1, "onEvent");
        if (duk_is_callable(ctx_, -1)) {

            PushWidgetEventObject(eventData);

            duk_call(ctx_, 1);

            if (duk_is_boolean(ctx_, -1) && duk_to_boolean(ctx_, -1))
                eventData[P_HANDLED] = true;
        }
        duk_pop_n(ctx_, 2);
        assert(top == duk_get_top(ctx_));
    }

    // specific event handlers
    if (type == tb::EVENT_TYPE_CLICK && handler == target)
    {
        int top = duk_get_top(ctx_);
        duk_push_heapptr(ctx_, handlerHeapPtr);
        duk_get_prop_string(ctx_, -1, "onClick");
        if (duk_is_callable(ctx_, -1)) {

            if (duk_pcall(ctx_, 0) != 0)
            {
                JSVM::GetJSVM(nullptr)->SendJSErrorEvent();
            }
            else
            {
                if (duk_is_boolean(ctx_, -1) && duk_to_boolean(ctx_, -1))
                    eventData[P_HANDLED] = true;
            }
        }
        duk_pop_n(ctx_, 2);
        assert(top == duk_get_top(ctx_));
        return;
    }

}

}
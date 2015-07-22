

#include "../Precompiled.h"

#include "../Core/Context.h"
#include "../Input/InputEvents.h"
#include "../UI/CheckBox.h"
#include "../UI/UIEvents.h"

#include "../DebugNew.h"

namespace Clockwork
{

extern const char* UI_CATEGORY;

CheckBox::CheckBox(Context* context) :
    BorderImage(context),
    checkedOffset_(IntVector2::ZERO),
    checked_(false)
{
    SetEnabled(true);
    focusMode_ = FM_FOCUSABLE_DEFOCUSABLE;
}

CheckBox::~CheckBox()
{
}

void CheckBox::RegisterObject(Context* context)
{
    context->RegisterFactory<CheckBox>(UI_CATEGORY);

    COPY_BASE_ATTRIBUTES(BorderImage);
    UPDATE_ATTRIBUTE_DEFAULT_VALUE("Is Enabled", true);
    UPDATE_ATTRIBUTE_DEFAULT_VALUE("Focus Mode", FM_FOCUSABLE_DEFOCUSABLE);
    ACCESSOR_ATTRIBUTE("Is Checked", IsChecked, SetChecked, bool, false, AM_FILE);
    ACCESSOR_ATTRIBUTE("Checked Image Offset", GetCheckedOffset, SetCheckedOffset, IntVector2, IntVector2::ZERO, AM_FILE);
}

void CheckBox::GetBatches(PODVector<UIBatch>& batches, PODVector<float>& vertexData, const IntRect& currentScissor)
{
    IntVector2 offset(IntVector2::ZERO);
    if (hovering_ || selected_ || HasFocus())
        offset += hoverOffset_;
    if (checked_)
        offset += checkedOffset_;

    BorderImage::GetBatches(batches, vertexData, currentScissor, offset);
}

void CheckBox::OnClickBegin(const IntVector2& position, const IntVector2& screenPosition, int button, int buttons, int qualifiers,
    Cursor* cursor)
{
    if (button == MOUSEB_LEFT && editable_)
        SetChecked(!checked_);
}

void CheckBox::OnKey(int key, int buttons, int qualifiers)
{
    if (HasFocus() && key == KEY_SPACE)
    {
        // Simulate LMB click
        OnClickBegin(IntVector2(), IntVector2(), MOUSEB_LEFT, 0, 0, 0);
    }
}

void CheckBox::SetChecked(bool enable)
{
    if (enable != checked_)
    {
        checked_ = enable;

        using namespace Toggled;

        VariantMap& eventData = GetEventDataMap();
        eventData[P_ELEMENT] = this;
        eventData[P_STATE] = checked_;
        SendEvent(E_TOGGLED, eventData);
    }
}

void CheckBox::SetCheckedOffset(const IntVector2& offset)
{
    checkedOffset_ = offset;
}

void CheckBox::SetCheckedOffset(int x, int y)
{
    checkedOffset_ = IntVector2(x, y);
}

}



#include "../Precompiled.h"

#include "../Core/Context.h"
#include "../UI/ToolTip.h"
#include "../UI/UI.h"

namespace Clockwork
{

extern const char* UI_CATEGORY;

ToolTip::ToolTip(Context* context) :
    UIElement(context),
    delay_(0.0f),
    parentHovered_(false)
{
    SetVisible(false);
}

ToolTip::~ToolTip()
{
}

void ToolTip::RegisterObject(Context* context)
{
    context->RegisterFactory<ToolTip>(UI_CATEGORY);

    COPY_BASE_ATTRIBUTES(UIElement);
    ACCESSOR_ATTRIBUTE("Delay", GetDelay, SetDelay, float, 0.0f, AM_FILE);
}

void ToolTip::Update(float timeStep)
{
    // Track the element we are parented to for hovering. When we display, we move ourself to the root element
    // to ensure displaying on top
    UIElement* root = GetRoot();
    if (!root)
        return;
    if (parent_ != root)
        target_ = parent_;

    // If target is removed while we are displaying, we have no choice but to destroy ourself
    if (target_.Expired())
    {
        Remove();
        return;
    }

    if (target_->IsHovering())
    {
        float effectiveDelay = delay_ > 0.0f ? delay_ : GetSubsystem<UI>()->GetDefaultToolTipDelay();

        if (!parentHovered_)
        {
            parentHovered_ = true;
            displayAt_.Reset();
        }
        else if (displayAt_.GetMSec(false) >= (unsigned)(effectiveDelay * 1000.0f) && parent_ == target_)
        {
            originalPosition_ = GetPosition();
            IntVector2 screenPosition = GetScreenPosition();
            SetParent(root);
            SetPosition(screenPosition);
            SetVisible(true);
            // BringToFront() is unreliable in this case as it takes into account only input-enabled elements.
            // Rather just force priority to max
            SetPriority(M_MAX_INT);
        }
    }
    else
    {
        if (IsVisible() && parent_ == root)
        {
            SetParent(target_);
            SetPosition(originalPosition_);
            SetVisible(false);
        }
        parentHovered_ = false;
        displayAt_.Reset();
    }
}

void ToolTip::SetDelay(float delay)
{
    delay_ = delay;
}

}
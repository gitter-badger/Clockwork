#pragma once

#include "../Graphics/GraphicsDefs.h"
#include "../UI/UIElement.h"

namespace Clockwork
{

/// Tooltip %UI element.
class CLOCKWORK_API ToolTip : public UIElement
{
    OBJECT(ToolTip)

public:
    /// Construct.
    ToolTip(Context* context);
    /// Destruct.
    virtual ~ToolTip();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Perform UI element update.
    virtual void Update(float timeStep);

    /// Set the delay in seconds until the tooltip shows once hovering. Set zero to use the default from the UI subsystem.
    void SetDelay(float delay);

    /// Return the delay in seconds until the tooltip shows once hovering.
    float GetDelay() const { return delay_; }

private:
    /// The element that is being tracked for hovering. Normally the parent element.
    WeakPtr<UIElement> target_;
    /// Delay from hover start to displaying the tooltip.
    float delay_;
    /// Point at which the parent was hovered.
    bool parentHovered_;
    /// Point at which the tooltip was set visible.
    Timer displayAt_;
    /// Original offset position to the parent.
    IntVector2 originalPosition_;
};

}

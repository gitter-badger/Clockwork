#pragma once

#include "../UI/UIElement.h"

namespace Clockwork
{

class Button;
class Slider;

/// Scroll bar %UI element with forward and back buttons.
class CLOCKWORK_API ScrollBar : public UIElement
{
    OBJECT(ScrollBar);

public:
    /// Construct.
    ScrollBar(Context* context);
    /// Destruct.
    virtual ~ScrollBar();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Apply attribute changes that can not be applied immediately.
    virtual void ApplyAttributes();
    /// React to resize.
    virtual void OnResize();
    /// React to editable status change.
    virtual void OnSetEditable();

    /// Set orientation type.
    void SetOrientation(Orientation orientation);
    /// Set slider range maximum value (minimum value is always 0.)
    void SetRange(float range);
    /// Set slider current value.
    void SetValue(float value);
    /// Change slider current value by a delta.
    void ChangeValue(float delta);
    /// Set button scroll step.
    void SetScrollStep(float step);
    /// Set button step factor, can be used to adjust the step for constant pixel size.
    void SetStepFactor(float factor);
    /// Scroll back one step.
    void StepBack();
    /// Scroll forward one step.
    void StepForward();

    /// Return scrollbar orientation.
    Orientation GetOrientation() const;
    /// Return slider range.
    float GetRange() const;
    /// Return slider current value.
    float GetValue() const;
    /// Return button scroll step.
    float GetScrollStep() const { return scrollStep_; }
    /// Return button step factor.
    float GetStepFactor() const { return stepFactor_; }
    /// Return scroll step multiplied by factor.
    float GetEffectiveScrollStep() const;
    /// Return back button element.
    Button* GetBackButton() const { return backButton_; }
    /// Return forward button element.
    Button* GetForwardButton() const { return forwardButton_; }
    /// Return slider element.
    Slider* GetSlider() const { return slider_; }

protected:
    /// Filter implicit attributes in serialization process.
    virtual bool FilterImplicitAttributes(XMLElement& dest) const;
    /// Filter implicit attributes in serialization process for internal button.
    bool FilterButtonImplicitAttributes(XMLElement& dest, const String& name) const;

    /// Back button.
    SharedPtr<Button> backButton_;
    /// Forward button.
    SharedPtr<Button> forwardButton_;
    /// Slider.
    SharedPtr<Slider> slider_;
    /// Scroll step.
    float scrollStep_;
    /// Step factor.
    float stepFactor_;
    /// Left button image rect.
    IntRect leftRect_;
    /// Right button image rect.
    IntRect rightRect_;
    /// Up button image rect.
    IntRect upRect_;
    /// Down button image rect.
    IntRect downRect_;

private:
    /// Handle back button pressed.
    void HandleBackButtonPressed(StringHash eventType, VariantMap& eventData);
    /// Handle forward button pressed.
    void HandleForwardButtonPressed(StringHash eventType, VariantMap& eventData);
    /// Handle slider movement.
    void HandleSliderChanged(StringHash eventType, VariantMap& eventData);
    /// Handle slider touch and click on "paging" area.
    void HandleSliderPaged(StringHash eventType, VariantMap& eventData);
};

}

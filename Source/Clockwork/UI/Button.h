#pragma once

#include "../UI/BorderImage.h"

namespace Clockwork
{

/// Pushbutton %UI element.
class CLOCKWORK_API Button : public BorderImage
{
    OBJECT(Button);

public:
    /// Construct.
    Button(Context* context);
    /// Destruct.
    virtual ~Button();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Perform UI element update.
    virtual void Update(float timeStep);
    /// Return UI rendering batches.
    virtual void GetBatches(PODVector<UIBatch>& batches, PODVector<float>& vertexData, const IntRect& currentScissor);
    /// React to mouse click begin.
    virtual void OnClickBegin(const IntVector2& position, const IntVector2& screenPosition, int button, int buttons, int qualifiers, Cursor* cursor);
    /// React to mouse click end.
    virtual void OnClickEnd(const IntVector2& position, const IntVector2& screenPosition, int button, int buttons, int qualifiers, Cursor* cursor, UIElement* beginElement);
    /// React to mouse drag motion.
    virtual void OnDragMove(const IntVector2& position, const IntVector2& screenPosition, const IntVector2& deltaPos, int buttons, int qualifiers, Cursor* cursor);
    /// React to a key press.
    virtual void OnKey(int key, int buttons, int qualifiers);

    /// Set offset to image rectangle used when pressed.
    void SetPressedOffset(const IntVector2& offset);
    /// Set offset to image rectangle used when pressed.
    void SetPressedOffset(int x, int y);
    /// Set offset of child elements when pressed.
    void SetPressedChildOffset(const IntVector2& offset);
    /// Set offset of child elements when pressed.
    void SetPressedChildOffset(int x, int y);
    /// Set repeat properties. Rate 0 (default) disables repeat.
    void SetRepeat(float delay, float rate);
    /// Set repeat delay.
    void SetRepeatDelay(float delay);
    /// Set repeat rate.
    void SetRepeatRate(float rate);

    /// Return pressed image offset.
    const IntVector2& GetPressedOffset() const { return pressedOffset_; }
    /// Return offset of child elements when pressed.
    const IntVector2& GetPressedChildOffset() const { return pressedChildOffset_; }
    /// Return repeat delay.
    float GetRepeatDelay() const { return repeatDelay_; }
    /// Return repeat rate.
    float GetRepeatRate() const { return repeatRate_; }
    /// Return whether is currently pressed.
    bool IsPressed() const { return pressed_; }

protected:
    /// Set new pressed state.
    void SetPressed(bool enable);

    /// Pressed image offset.
    IntVector2 pressedOffset_;
    /// Pressed label offset.
    IntVector2 pressedChildOffset_;
    /// Repeat delay.
    float repeatDelay_;
    /// Repeat rate.
    float repeatRate_;
    /// Repeat timer.
    float repeatTimer_;
    /// Current pressed state.
    bool pressed_;
};

}

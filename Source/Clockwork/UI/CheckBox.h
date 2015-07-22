

#pragma once

#include "../UI/BorderImage.h"

namespace Clockwork
{

/// %UI element that can be toggled between unchecked and checked state.
class CLOCKWORK_API CheckBox : public BorderImage
{
    OBJECT(CheckBox);

public:
    /// Construct.
    CheckBox(Context* context);
    /// Destruct.
    virtual ~CheckBox();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Return UI rendering batches.
    virtual void GetBatches(PODVector<UIBatch>& batches, PODVector<float>& vertexData, const IntRect& currentScissor);
    /// React to mouse click begin.
    virtual void OnClickBegin
        (const IntVector2& position, const IntVector2& screenPosition, int button, int buttons, int qualifiers, Cursor* cursor);
    /// React to a key press.
    virtual void OnKey(int key, int buttons, int qualifiers);

    /// Set checked state.
    void SetChecked(bool enable);
    /// Set checked image offset.
    void SetCheckedOffset(const IntVector2& rect);
    /// Set checked image offset.
    void SetCheckedOffset(int x, int y);

    /// Return whether is checked.
    bool IsChecked() const { return checked_; }

    /// Return checked image offset.
    const IntVector2& GetCheckedOffset() const { return checkedOffset_; }

protected:
    /// Checked image offset.
    IntVector2 checkedOffset_;
    /// Current checked state.
    bool checked_;
};

}

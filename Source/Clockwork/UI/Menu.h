#pragma once

#include "../UI/Button.h"

namespace Clockwork
{

/// %Menu %UI element that optionally shows a popup.
class CLOCKWORK_API Menu : public Button
{
    OBJECT(Menu);

    using UIElement::LoadXML;

public:
    /// Construct.
    Menu(Context* context);
    /// Destruct.
    virtual ~Menu();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Load from XML data with style. Return true if successful.
    virtual bool LoadXML(const XMLElement& source, XMLFile* styleFile, bool setInstanceDefault = false);
    /// Save as XML data. Return true if successful.
    virtual bool SaveXML(XMLElement& dest) const;

    /// Perform UI element update.
    virtual void Update(float timeStep);
    /// React to mouse hover.
    virtual void OnHover(const IntVector2& position, const IntVector2& screenPosition, int buttons, int qualifiers, Cursor* cursor);
    /// React to the popup being shown.
    virtual void OnShowPopup();
    /// React to the popup being hidden.
    virtual void OnHidePopup() {}

    /// Set popup element to show on selection.
    void SetPopup(UIElement* element);
    /// Set popup element offset.
    void SetPopupOffset(const IntVector2& offset);
    /// Set popup element offset.
    void SetPopupOffset(int x, int y);
    /// Force the popup to show or hide.
    void ShowPopup(bool enable);
    /// Set accelerator key (set zero key code to disable.)
    void SetAccelerator(int key, int qualifiers);

    /// Return popup element.
    UIElement* GetPopup() const { return popup_; }
    /// Return popup element offset.
    const IntVector2& GetPopupOffset() const { return popupOffset_; }
    /// Return whether popup is open.
    bool GetShowPopup() const { return showPopup_; }
    /// Return accelerator key code, 0 if disabled.
    int GetAcceleratorKey() const { return acceleratorKey_; }
    /// Return accelerator qualifiers.
    int GetAcceleratorQualifiers() const { return acceleratorQualifiers_; }

protected:
    /// Filter implicit attributes in serialization process.
    virtual bool FilterPopupImplicitAttributes(XMLElement& dest) const;
    /// Popup element.
    SharedPtr<UIElement> popup_;
    /// Popup element offset.
    IntVector2 popupOffset_;
    /// Show popup flag.
    bool showPopup_;
    /// Accelerator key code.
    int acceleratorKey_;
    /// Accelerator qualifiers.
    int acceleratorQualifiers_;

private:
    /// Handle press and release for selection and toggling popup visibility.
    void HandlePressedReleased(StringHash eventType, VariantMap& eventData);
    /// Handle global focus change to check for hiding the popup.
    void HandleFocusChanged(StringHash eventType, VariantMap& eventData);
    /// Handle keypress for checking accelerator.
    void HandleKeyDown(StringHash eventType, VariantMap& eventData);
    /// Auto popup flag.
    bool autoPopup_;
};

}



#pragma once

#include "../Core/Object.h"

namespace Clockwork
{

class Button;
class Text;
class UIElement;
class XMLFile;

/// Message box dialog.
class CLOCKWORK_API MessageBox : public Object
{
    OBJECT(MessageBox);

public:
    /// Construct. If layout file is not given, use the default message box layout. If style file is not given, use the default style file from root UI element.
    MessageBox(Context* context, const String& messageString = String::EMPTY, const String& titleString = String::EMPTY,
        XMLFile* layoutFile = 0, XMLFile* styleFile = 0);
    /// Destruct.
    virtual ~MessageBox();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Set title text. No-ops if there is no title text element.
    void SetTitle(const String& text);
    /// Set message text. No-ops if there is no message text element.
    void SetMessage(const String& text);

    /// Return title text. Return empty string if there is no title text element.
    const String& GetTitle() const;
    /// Return message text. Return empty string if there is no message text element.
    const String& GetMessage() const;

    /// Return dialog window.
    UIElement* GetWindow() const { return window_; }

private:
    /// Handle events that dismiss the message box.
    void HandleMessageAcknowledged(StringHash eventType, VariantMap& eventData);

    /// UI element containing the whole UI layout. Typically it is a Window element type.
    SharedPtr<UIElement> window_;
    /// Title text element.
    Text* titleText_;
    /// Message text element.
    Text* messageText_;
    /// OK button element.
    Button* okButton_;
};

}

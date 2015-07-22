

#include "../Precompiled.h"

#include "../Core/Context.h"
#include "../Graphics/Graphics.h"
#include "../IO/Log.h"
#include "../Resource/ResourceCache.h"
#include "../Resource/XMLFile.h"
#include "../UI/Button.h"
#include "../UI/MessageBox.h"
#include "../UI/Text.h"
#include "../UI/UI.h"
#include "../UI/UIEvents.h"
#include "../UI/Window.h"

namespace Clockwork
{

MessageBox::MessageBox(Context* context, const String& messageString, const String& titleString, XMLFile* layoutFile,
    XMLFile* styleFile) :
    Object(context),
    titleText_(0),
    messageText_(0),
    okButton_(0)
{
    // If layout file is not given, use the default message box layout
    if (!layoutFile)
    {
        ResourceCache* cache = GetSubsystem<ResourceCache>();
        layoutFile = cache->GetResource<XMLFile>("UI/MessageBox.xml");
        if (!layoutFile)    // Error is already logged
            return;         // Note: windowless MessageBox should not be used!
    }

    UI* ui = GetSubsystem<UI>();
    window_ = ui->LoadLayout(layoutFile, styleFile);
    if (!window_)   // Error is already logged
        return;
    ui->GetRoot()->AddChild(window_);

    // Set the title and message strings if they are given
    titleText_ = dynamic_cast<Text*>(window_->GetChild("TitleText", true));
    if (titleText_ && !titleString.Empty())
        titleText_->SetText(titleString);
    messageText_ = dynamic_cast<Text*>(window_->GetChild("MessageText", true));
    if (messageText_ && !messageString.Empty())
        messageText_->SetText(messageString);

    // Center window after the message is set
    Window* window = dynamic_cast<Window*>(window_.Get());
    if (window)
    {
        Graphics* graphics = GetSubsystem<Graphics>();  // May be null if headless
        if (graphics)
        {
            const IntVector2& size = window->GetSize();
            window->SetPosition((graphics->GetWidth() - size.x_) / 2, (graphics->GetHeight() - size.y_) / 2);
        }
        else
            LOGWARNING("Instantiating a modal window in headless mode!");

        window->SetModal(true);
        SubscribeToEvent(window, E_MODALCHANGED, HANDLER(MessageBox, HandleMessageAcknowledged));
    }

    // Bind the buttons (if any in the loaded UI layout) to event handlers
    okButton_ = dynamic_cast<Button*>(window_->GetChild("OkButton", true));
    if (okButton_)
    {
        ui->SetFocusElement(okButton_);
        SubscribeToEvent(okButton_, E_RELEASED, HANDLER(MessageBox, HandleMessageAcknowledged));
    }
    Button* cancelButton = dynamic_cast<Button*>(window_->GetChild("CancelButton", true));
    if (cancelButton)
        SubscribeToEvent(cancelButton, E_RELEASED, HANDLER(MessageBox, HandleMessageAcknowledged));
    Button* closeButton = dynamic_cast<Button*>(window_->GetChild("CloseButton", true));
    if (closeButton)
        SubscribeToEvent(closeButton, E_RELEASED, HANDLER(MessageBox, HandleMessageAcknowledged));
}

MessageBox::~MessageBox()
{
    if (window_)
        window_->Remove();
}

void MessageBox::RegisterObject(Context* context)
{
    context->RegisterFactory<MessageBox>();
}

void MessageBox::SetTitle(const String& text)
{
    if (titleText_)
        titleText_->SetText(text);
}

void MessageBox::SetMessage(const String& text)
{
    if (messageText_)
        messageText_->SetText(text);
}

const String& MessageBox::GetTitle() const
{
    return titleText_ ? titleText_->GetText() : String::EMPTY;
}

const String& MessageBox::GetMessage() const
{
    return messageText_ ? messageText_->GetText() : String::EMPTY;
}

void MessageBox::HandleMessageAcknowledged(StringHash eventType, VariantMap& eventData)
{
    using namespace MessageACK;

    VariantMap& newEventData = GetEventDataMap();
    newEventData[P_OK] = eventData[Released::P_ELEMENT] == okButton_;
    SendEvent(E_MESSAGEACK, newEventData);

    this->ReleaseRef();
}

}

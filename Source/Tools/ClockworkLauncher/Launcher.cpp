#include "Launcher.h"

#include <Clockwork/Clockwork.h>
#include <Clockwork/Engine/Application.h>
#include <Clockwork/Engine/Engine.h>
#include <Clockwork/Input/Input.h>
#include <Clockwork/Input/InputEvents.h>
#include <Clockwork/Graphics/Graphics.h>
#include <Clockwork/Resource/ResourceCache.h>
#include <Clockwork/Resource/Image.h>
#include <Clockwork/UI/UI.h>
#include <Clockwork/UI/UIEvents.h>
#include <Clockwork/UI/Window.h>
#include <Clockwork/UI/Text.h>
#include <Clockwork/UI/Button.h>
#include <Clockwork/UI/LineEdit.h>
#include <Clockwork/Graphics/Texture2D.h>
#include <Clockwork/UI/ListView.h>
#include <Clockwork/>


DEFINE_APPLICATION_MAIN(ClockworkLauncher);

ClockworkLauncher::ClockworkLauncher(Context* context) :
	Application(context),
	uiRoot_(GetSubsystem<UI>()->GetRoot())
{

}

void ClockworkLauncher::Setup()
{
	engineParameters_["WindowTitle"] = "Clockwork Editor";
	engineParameters_["FullScreen"] = false;
	engineParameters_["Headless"] = false;
	engineParameters_["Headless"] = false;
	engineParameters_["WindowWidth"] = 800;
	engineParameters_["WindowHeight"] = 600;
	engineParameters_["WindowResizable"] = false;
	engineParameters_["Borderless"] = false;
	//engineParameters_
}

void ClockworkLauncher::Start()
{
	// Enable OS cursor
	GetSubsystem<Input>()->SetMouseVisible(true);

	// Load XML file containing default UI style sheet
	ResourceCache* cache = GetSubsystem<ResourceCache>();
	XMLFile* style = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");


	// Set the loaded style as default style
	uiRoot_->SetDefaultStyle(style);

	// Initialize Window
	InitWindow();

	// Create and add some controls to the Window
	InitControls();

	SubscribeToEvent(E_KEYDOWN, HANDLER(ClockworkLauncher, HandleKeyDown));
}

void ClockworkLauncher::Stop()
{

}

void ClockworkLauncher::InitControls()
{
	ResourceCache* cache = GetSubsystem<ResourceCache>();

	// Create Clockwork Logo in Launcher
	Button* button = new Button(context_);
	button->SetTexture(cache->GetResource<Texture2D>("Textures/ClockworkIcon.png"));
	button->SetAlignment(HA_LEFT, VA_TOP);
	button->SetSize(IntVector2(128,128));
	button->SetVisible(true);
	button->SetMinSize(IntVector2(128, 128));
	button->SetMaxSize(IntVector2(128, 128));

	// Create the Window title Text
	Text* windowTitle = new Text(context_);
	windowTitle->SetName("WindowTitle");
	windowTitle->SetText("Clockwork Project Launcher");
	windowTitle->SetFont(windowTitle->GetFont(),100);
	windowTitle->SetPosition(IntVector2(400, 50));
	windowTitle->SetHorizontalAlignment(HA_RIGHT);


	// Create a LineEdit
	LineEdit* lineEdit = new LineEdit(context_);
	lineEdit->SetName("LineEdit");
	lineEdit->SetMinHeight(30);

	// Add items to the window
	window_->AddChild(button);
	button->AddChild(windowTitle);
	window_->AddChild(lineEdit);

	windowTitle->SetStyleAuto();
	lineEdit->SetStyleAuto();

	windowTitle->SetColor(Color(0.0f, 0.0f, 0.0f));

}

void ClockworkLauncher::InitWindow()
{
	// Create the Window and add it to the UI's root node
	window_ = new Window(context_);
	uiRoot_->AddChild(window_);

	// Set Window size and layout settings
	window_->SetMinSize(800, 600);
	window_->SetLayout(LM_VERTICAL, 6, IntRect(6, 6, 6, 6));
	window_->SetAlignment(HA_LEFT, VA_TOP);
	window_->SetName("ClockworkLauncher");

	// Create Window 'titlebar' container
	UIElement* titleBar = new UIElement(context_);
	titleBar->SetVerticalAlignment(VA_TOP);
	titleBar->SetLayoutMode(LM_HORIZONTAL);

	// Add the title bar to the Window
	window_->AddChild(titleBar);

	// Apply styles
	window_->SetStyleAuto();
}

void ClockworkLauncher::InitProjectList()
{

}

void ClockworkLauncher::SetIconAndTitle()
{
	ResourceCache* cache = GetSubsystem<ResourceCache>();
	Graphics* graphics = GetSubsystem<Graphics>();

	// Load Icon
	Image* icon = cache->GetResource<Image>("Textures/ClockworkIcon.png");

	//Set Icon and Title
	graphics->SetWindowIcon(icon);
	graphics->SetWindowTitle("Clockwork Editor");

}

void ClockworkLauncher::HandleKeyDown(StringHash eventType, VariantMap& eventData)
{
	using namespace KeyDown;
	// Check for pressing ESC. Note the engine_ member variable for convenience access to the Engine object
	int key = eventData[P_KEY].GetInt();
	if (key == KEY_ESC)
	{
		engine_->Exit();
	}
}
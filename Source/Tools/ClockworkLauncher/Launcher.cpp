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
	engineParameters_["Borderless"] = true;
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

	SetIconAndTitle();

	SubscribeToEvent(E_KEYDOWN, HANDLER(ClockworkLauncher, HandleKeyDown));
}

void ClockworkLauncher::Stop()
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
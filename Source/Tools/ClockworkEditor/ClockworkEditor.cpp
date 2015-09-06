#include "ClockworkEditor.h"

#include <Clockwork/Clockwork.h>
#include <Clockwork/Engine/Application.h>
#include <Clockwork/Engine/Engine.h>
#include <Clockwork/Input/InputEvents.h>
#include <Clockwork/Graphics/Graphics.h>
#include <Clockwork/Resource/ResourceCache.h>
#include <Clockwork/Resource/Image.h>

DEFINE_APPLICATION_MAIN(ClockworkEditor);

ClockworkEditor::ClockworkEditor(Context* context):
	Application(context)
{

}

void ClockworkEditor::Setup()
{
	engineParameters_["WindowTitle"] = "Clockwork Editor";
	engineParameters_["FullScreen"] = false;
	engineParameters_["Headless"] = false;
}

void ClockworkEditor::Start()
{
	ResourceCache* cache = GetSubsystem<ResourceCache>();
	Graphics* graphics = GetSubsystem<Graphics>();
	Image* icon = cache->GetResource<Image>("Textures/ClockworkIcon.png");
	graphics->SetWindowIcon(icon);
	graphics->SetWindowTitle("Clockwork Editor");

	SubscribeToEvent(E_KEYDOWN, HANDLER(ClockworkEditor, HandleKeyDown));
}

void ClockworkEditor::Stop()
{

}

void ClockworkEditor::HandleKeyDown(StringHash eventType, VariantMap& eventData)
{
	using namespace KeyDown;
	// Check for pressing ESC. Note the engine_ member variable for convenience access to the Engine object
	int key = eventData[P_KEY].GetInt();
	if (key == KEY_ESC)
	{
		engine_->Exit();
	}
}
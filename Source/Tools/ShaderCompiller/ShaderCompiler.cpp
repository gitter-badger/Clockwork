#include "ShaderCompiler.h"
#include <Clockwork/Clockwork.h>
#include <Clockwork/Engine/Application.h>
#include <Clockwork/Engine/Engine.h>
#include <Clockwork/Input/Input.h>
#include <Clockwork/Resource/ResourceCache.h>
#include <Clockwork/Graphics/Graphics.h>
#include <Clockwork/IO/FileWatcher.h>
#include <Clockwork/IO/FileSystem.h>
#include <Clockwork/IO/File.h>

DEFINE_APPLICATION_MAIN(ShaderCompiler);

ShaderCompiler::ShaderCompiler(Context* context) :
	Application(context)
{
	
}

void ShaderCompiler::Setup()
{
	engineParameters_["WindowTitle"] = "Clockwork Editor";
	engineParameters_["FullScreen"] = false;
	engineParameters_["Headless"] = false;
	engineParameters_["WindowResizable"] = true;
	engineParameters_["Borderless"] = false;
	engineParameters_["ResourcePaths"] = "../Data;../CoreData";
}

void ShaderCompiler::Start()
{
	// Enable OS cursor
	GetSubsystem<Input>()->SetMouseVisible(true);

	SetIconAndTitle();

	SubscribeToEvent(E_KEYDOWN, HANDLER(ShaderCompiler, HandleKeyDown));
	LoadSahders();
}

void ShaderCompiler::Stop()
{
	
}

void ShaderCompiler::SetIconAndTitle()
{
	auto cache = GetSubsystem<ResourceCache>();
	auto graphics = GetSubsystem<Graphics>();

	// Load Icon
	auto icon = cache->GetResource<Image>("Textures/ClockworkIcon.png");

	//Set Icon and Title
	graphics->SetWindowIcon(icon);
	graphics->SetWindowTitle("Clockwork Editor");

}

void ShaderCompiler::LoadSahders()
{
	FileSystem* fileSystem = new FileSystem(context_);
	if(!fileSystem->DirExists("../Shaders"))
		fileSystem->CreateDir("../Shaders");

	
}


void ShaderCompiler::HandleKeyDown(StringHash eventType, VariantMap& eventData)
{
	using namespace KeyDown;
	// Check for pressing ESC. Note the engine_ member variable for convenience access to the Engine object
	int key = eventData[P_KEY].GetInt();
	if (key == KEY_ESC)
	{
		engine_->Exit();
	}
}
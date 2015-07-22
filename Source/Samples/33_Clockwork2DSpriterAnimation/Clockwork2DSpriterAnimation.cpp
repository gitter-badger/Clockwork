

#include <Clockwork/Clockwork.h>

#include <Clockwork/Clockwork2D/AnimatedSprite2D.h>
#include <Clockwork/Clockwork2D/AnimationSet2D.h>
#include <Clockwork/Graphics/Camera.h>
#include <Clockwork/Core/CoreEvents.h>
#include <Clockwork/Clockwork2D/Drawable2D.h>
#include <Clockwork/Engine/Engine.h>
#include <Clockwork/UI/Font.h>
#include <Clockwork/Graphics/Graphics.h>
#include <Clockwork/Input/Input.h>
#include <Clockwork/Graphics/Octree.h>
#include <Clockwork/Graphics/Renderer.h>
#include <Clockwork/Resource/ResourceCache.h>
#include <Clockwork/Scene/Scene.h>
#include <Clockwork/UI/Text.h>
#include <Clockwork/Graphics/Zone.h>

#include "Clockwork2DSpriterAnimation.h"

#include <Clockwork/DebugNew.h>

static const char* animationNames[] =
{
    "idle",
    "run",
    "attack",
    "hit",
    "dead",
    "dead2",
    "dead3",
};

DEFINE_APPLICATION_MAIN(Clockwork2DSpriterAnimation)

Clockwork2DSpriterAnimation::Clockwork2DSpriterAnimation(Context* context) :
    Sample(context),
    animationIndex_(0)
{
}

void Clockwork2DSpriterAnimation::Start()
{
    // Execute base class startup
    Sample::Start();

    // Create the scene content
    CreateScene();

    // Create the UI content
    CreateInstructions();

    // Setup the viewport for displaying the scene
    SetupViewport();

    // Hook up to the frame update events
    SubscribeToEvents();
}

void Clockwork2DSpriterAnimation::CreateScene()
{
    scene_ = new Scene(context_);
    scene_->CreateComponent<Octree>();

    // Create camera node
    cameraNode_ = scene_->CreateChild("Camera");
    // Set camera's position
    cameraNode_->SetPosition(Vector3(0.0f, 0.0f, -10.0f));

    Camera* camera = cameraNode_->CreateComponent<Camera>();
    camera->SetOrthographic(true);

    Graphics* graphics = GetSubsystem<Graphics>();
    camera->SetOrthoSize((float)graphics->GetHeight() * PIXEL_SIZE);
    camera->SetZoom(1.5f * Min((float)graphics->GetWidth() / 1280.0f, (float)graphics->GetHeight() / 800.0f)); // Set zoom according to user's resolution to ensure full visibility (initial zoom (1.5) is set for full visibility at 1280x800 resolution)

    ResourceCache* cache = GetSubsystem<ResourceCache>();  
    AnimationSet2D* animationSet = cache->GetResource<AnimationSet2D>("Clockwork2D/imp/imp.scml");
    if (!animationSet)
        return;

    spriteNode_ = scene_->CreateChild("SpriterAnimation");

    AnimatedSprite2D* animatedSprite = spriteNode_->CreateComponent<AnimatedSprite2D>();
    animatedSprite->SetAnimation(animationSet, animationNames[animationIndex_]);
}

void Clockwork2DSpriterAnimation::CreateInstructions()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    UI* ui = GetSubsystem<UI>();

    // Construct new Text object, set string to display and font to use
    Text* instructionText = ui->GetRoot()->CreateChild<Text>();
    instructionText->SetText("Mouse click to play next animation, \nUse WASD keys to move, use PageUp PageDown keys to zoom.");
    instructionText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 15);
    instructionText->SetTextAlignment(HA_CENTER); // Center rows in relation to each other

    // Position the text relative to the screen center
    instructionText->SetHorizontalAlignment(HA_CENTER);
    instructionText->SetVerticalAlignment(VA_CENTER);
    instructionText->SetPosition(0, ui->GetRoot()->GetHeight() / 4);
}

void Clockwork2DSpriterAnimation::SetupViewport()
{
    Renderer* renderer = GetSubsystem<Renderer>();

    // Set up a viewport to the Renderer subsystem so that the 3D scene can be seen
    SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
    renderer->SetViewport(0, viewport);
}

void Clockwork2DSpriterAnimation::MoveCamera(float timeStep)
{
    // Do not move if the UI has a focused element (the console)
    if (GetSubsystem<UI>()->GetFocusElement())
        return;

    Input* input = GetSubsystem<Input>();

    // Movement speed as world units per second
    const float MOVE_SPEED = 4.0f;

    // Read WASD keys and move the camera scene node to the corresponding direction if they are pressed
    if (input->GetKeyDown('W'))
        cameraNode_->Translate(Vector3::UP * MOVE_SPEED * timeStep);
    if (input->GetKeyDown('S'))
        cameraNode_->Translate(Vector3::DOWN * MOVE_SPEED * timeStep);
    if (input->GetKeyDown('A'))
        cameraNode_->Translate(Vector3::LEFT * MOVE_SPEED * timeStep);
    if (input->GetKeyDown('D'))
        cameraNode_->Translate(Vector3::RIGHT * MOVE_SPEED * timeStep);

    if (input->GetKeyDown(KEY_PAGEUP))
    {
        Camera* camera = cameraNode_->GetComponent<Camera>();
        camera->SetZoom(camera->GetZoom() * 1.01f);
    }

    if (input->GetKeyDown(KEY_PAGEDOWN))
    {
        Camera* camera = cameraNode_->GetComponent<Camera>();
        camera->SetZoom(camera->GetZoom() * 0.99f);
    }
}

void Clockwork2DSpriterAnimation::SubscribeToEvents()
{
    // Subscribe HandleUpdate() function for processing update events
    SubscribeToEvent(E_UPDATE, HANDLER(Clockwork2DSpriterAnimation, HandleUpdate));
    SubscribeToEvent(E_MOUSEBUTTONDOWN, HANDLER(Clockwork2DSpriterAnimation, HandleMouseButtonDown));


    // Unsubscribe the SceneUpdate event from base class to prevent camera pitch and yaw in 2D sample
    UnsubscribeFromEvent(E_SCENEUPDATE);
}

void Clockwork2DSpriterAnimation::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace Update;

    // Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    // Move the camera, scale movement with time step
    MoveCamera(timeStep);
}

void Clockwork2DSpriterAnimation::HandleMouseButtonDown(StringHash eventType, VariantMap& eventData)
{
    AnimatedSprite2D* animatedSprite = spriteNode_->GetComponent<AnimatedSprite2D>();
    animationIndex_ = (animationIndex_ + 1) % 7;
    animatedSprite->SetAnimation(animationNames[animationIndex_], LM_FORCE_LOOPED);
}

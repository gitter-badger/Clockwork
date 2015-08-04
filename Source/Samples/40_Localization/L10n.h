

#pragma once

#include "Sample.h"

/// Localization example.
/// This sample demonstrates:
///     - Loading a collection of strings from JSON-files
///     - Creating text elements that automatically translates itself by changing the language
///     - The manually reaction to change language
class L10n : public Sample
{
    OBJECT(L10n);

public:
    /// Construct.
    L10n(Context* context);

    /// Setup after engine initialization and before running the main loop.
    virtual void Start();

private:
    // Load strings from JSON files and subscribe to the change language event
    void InitLocalizationSystem();
    // Init the 3D space
    void CreateScene();
    // Init the user interface
    void CreateGUI();
    /// Handle the logic update event.
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    // Handle the change language event.
    void HandleChangeLanguage(StringHash eventType, VariantMap& eventData);
    // Hook up to the buttons pressing
    void HandleChangeLangButtonPressed(StringHash eventType, VariantMap& eventData);
    void HandleQuitButtonPressed(StringHash eventType, VariantMap& eventData);
};



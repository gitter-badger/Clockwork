#pragma once

#include "../Scene/Component.h"

namespace Clockwork
{

/// %Sound listener component
class CLOCKWORK_API SoundListener : public Component
{
    OBJECT(SoundListener);

public:
    /// Construct.
    SoundListener(Context* context);
    /// Destruct.
    virtual ~SoundListener();
    /// Register object factory.
    static void RegisterObject(Context* context);
};

}

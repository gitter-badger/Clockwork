

#include "../Precompiled.h"

#include "../Audio/SoundListener.h"
#include "../Core/Context.h"

namespace Clockwork
{

extern const char* AUDIO_CATEGORY;

SoundListener::SoundListener(Context* context) :
    Component(context)
{
}

SoundListener::~SoundListener()
{
}

void SoundListener::RegisterObject(Context* context)
{
    context->RegisterFactory<SoundListener>(AUDIO_CATEGORY);

    ACCESSOR_ATTRIBUTE("Is Enabled", IsEnabled, SetEnabled, bool, true, AM_DEFAULT);
}

}

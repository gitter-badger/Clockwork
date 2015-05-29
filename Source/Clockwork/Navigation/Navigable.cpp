#include "../Core/Context.h"
#include "../Navigation/Navigable.h"

#include "../DebugNew.h"

namespace Clockwork
{

extern const char* NAVIGATION_CATEGORY;

Navigable::Navigable(Context* context) :
    Component(context),
    recursive_(true)
{
}

Navigable::~Navigable()
{
}

void Navigable::RegisterObject(Context* context)
{
    context->RegisterFactory<Navigable>(NAVIGATION_CATEGORY);

    ACCESSOR_ATTRIBUTE("Is Enabled", IsEnabled, SetEnabled, bool, true, AM_DEFAULT);
    ATTRIBUTE("Recursive", bool, recursive_, true, AM_DEFAULT);
}

void Navigable::SetRecursive(bool enable)
{
    recursive_ = enable;
}

}



#include "../Precompiled.h"

#include "../Input/Controls.h"

namespace Clockwork
{

Controls::Controls() :
    buttons_(0),
    yaw_(0.f),
    pitch_(0.f)
{
}

Controls::~Controls()
{
}

void Controls::Reset()
{
    buttons_ = 0;
    yaw_ = 0.0f;
    pitch_ = 0.0f;
    extraData_.Clear();
}

}

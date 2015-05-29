#include "../../Graphics/Graphics.h"
#include "../../Graphics/GraphicsImpl.h"

#include "../../DebugNew.h"

namespace Clockwork
{

GraphicsImpl::GraphicsImpl() :
    window_(0),
    context_(0),
    systemFBO_(0),
    activeTexture_(0),
    enabledAttributes_(0),
    boundFBO_(0),
    boundVBO_(0),
    boundUBO_(0),
    pixelFormat_(0),
    fboDirty_(false)
{
}

}

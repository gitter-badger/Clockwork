

#pragma once

#ifdef CLOCKWORK_IS_BUILDING
#include "Clockwork.h"
#else
#include <Clockwork/Clockwork.h>
#endif

namespace Clockwork
{

/// Return subversion revision number as of the library build time.
CLOCKWORK_API const char* GetRevision();

}

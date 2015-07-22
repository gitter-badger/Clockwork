

#pragma once

namespace Clockwork
{

#if defined(_MSC_VER) && defined(CLOCKWORK_MINIDUMPS)
/// Write a minidump. Needs to be called from within a structured exception handler.
CLOCKWORK_API int WriteMiniDump(const char* applicationName, void* exceptionPointers);
#endif

}


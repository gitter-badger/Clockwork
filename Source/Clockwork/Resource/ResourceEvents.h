

#pragma once

#include "../Core/Object.h"

namespace Clockwork
{

/// Resource reloading started.
EVENT(E_RELOADSTARTED, ReloadStarted)
{
}

/// Resource reloading finished successfully.
EVENT(E_RELOADFINISHED, ReloadFinished)
{
}

/// Resource reloading failed.
EVENT(E_RELOADFAILED, ReloadFailed)
{
}

/// Tracked file changed in the resource directories.
EVENT(E_FILECHANGED, FileChanged)
{
    PARAM(P_FILENAME, FileName);                    // String
    PARAM(P_RESOURCENAME, ResourceName);            // String
}

/// Resource loading failed.
EVENT(E_LOADFAILED, LoadFailed)
{
    PARAM(P_RESOURCENAME, ResourceName);            // String
}

/// Resource not found.
EVENT(E_RESOURCENOTFOUND, ResourceNotFound)
{
    PARAM(P_RESOURCENAME, ResourceName);            // String
}

/// Unknown resource type.
EVENT(E_UNKNOWNRESOURCETYPE, UnknownResourceType)
{
    PARAM(P_RESOURCETYPE, ResourceType);            // StringHash
}

/// Resource background loading finished.
EVENT(E_RESOURCEBACKGROUNDLOADED, ResourceBackgroundLoaded)
{
    PARAM(P_RESOURCENAME, ResourceName);            // String
    PARAM(P_SUCCESS, Success);                      // bool
    PARAM(P_RESOURCE, Resource);                    // Resource pointer
}

/// Language changed.
EVENT(E_CHANGELANGUAGE, ChangeLanguage)
{
}

}

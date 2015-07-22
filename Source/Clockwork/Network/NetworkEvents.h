

#pragma once

#include "../Core/Object.h"

namespace Clockwork
{

/// Server connection established.
EVENT(E_SERVERCONNECTED, ServerConnected)
{
}

/// Server connection disconnected.
EVENT(E_SERVERDISCONNECTED, ServerDisconnected)
{
}

/// Server connection failed.
EVENT(E_CONNECTFAILED, ConnectFailed)
{
}

/// New client connection established.
EVENT(E_CLIENTCONNECTED, ClientConnected)
{
    PARAM(P_CONNECTION, Connection);        // Connection pointer
}

/// Client connection disconnected.
EVENT(E_CLIENTDISCONNECTED, ClientDisconnected)
{
    PARAM(P_CONNECTION, Connection);        // Connection pointer
}

/// Client has sent identity: identity map is in the event data.
EVENT(E_CLIENTIDENTITY, ClientIdentity)
{
    PARAM(P_CONNECTION, Connection);        // Connection pointer
    PARAM(P_ALLOW, Allow);                  // bool
}

/// Client has informed to have loaded the scene.
EVENT(E_CLIENTSCENELOADED, ClientSceneLoaded)
{
    PARAM(P_CONNECTION, Connection);        // Connection pointer
}

/// Unhandled network message received.
EVENT(E_NETWORKMESSAGE, NetworkMessage)
{
    PARAM(P_CONNECTION, Connection);        // Connection pointer
    PARAM(P_MESSAGEID, MessageID);          // int
    PARAM(P_DATA, Data);                    // Buffer
}

/// About to send network update on the client or server.
EVENT(E_NETWORKUPDATE, NetworkUpdate)
{
}

/// Network update has been sent on the client or server.
EVENT(E_NETWORKUPDATESENT, NetworkUpdateSent)
{
}

/// Scene load failed, either due to file not found or checksum error.
EVENT(E_NETWORKSCENELOADFAILED, NetworkSceneLoadFailed)
{
    PARAM(P_CONNECTION, Connection);      // Connection pointer
}

/// Remote event: adds Connection parameter to the event data
EVENT(E_REMOTEEVENTDATA, RemoteEventData)
{
    PARAM(P_CONNECTION, Connection);      // Connection pointer
}

}

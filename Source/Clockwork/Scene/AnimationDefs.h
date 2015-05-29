#pragma once

namespace Clockwork
{

/// Animation wrap mode.
enum WrapMode
{
    /// Loop mode.
    WM_LOOP = 0,
    /// Play once, when animation finished it will be removed.
    WM_ONCE,
    /// Clamp mode.
    WM_CLAMP,
};

}

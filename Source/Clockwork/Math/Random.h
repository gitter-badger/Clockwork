#pragma once

namespace Clockwork
{

/// Set the random seed. The default seed is 1.
CLOCKWORK_API void SetRandomSeed(unsigned seed);
/// Return the current random seed.
CLOCKWORK_API unsigned GetRandomSeed();
/// Return a random number between 0-32767. Should operate similarly to MSVC rand().
CLOCKWORK_API int Rand();
/// Return a standard normal distributed number.
CLOCKWORK_API float RandStandardNormal();

}

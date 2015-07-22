

#pragma once

#include "../Audio/SoundStream.h"
#include "../Container/ArrayPtr.h"

namespace Clockwork
{

class Sound;

/// Ogg Vorbis sound stream.
class CLOCKWORK_API OggVorbisSoundStream : public SoundStream
{
public:
    /// Construct from an Ogg Vorbis compressed sound.
    OggVorbisSoundStream(const Sound* sound);
    /// Destruct.
    ~OggVorbisSoundStream();

    /// Produce sound data into destination. Return number of bytes produced. Called by SoundSource from the mixing thread.
    virtual unsigned GetData(signed char* dest, unsigned numBytes);

protected:
    /// Decoder state.
    void* decoder_;
    /// Compressed sound data.
    SharedArrayPtr<signed char> data_;
    /// Compressed sound data size in bytes.
    unsigned dataSize_;
};

}

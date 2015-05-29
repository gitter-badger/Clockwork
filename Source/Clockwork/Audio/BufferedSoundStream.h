#pragma once

#include "../Container/ArrayPtr.h"
#include "../Container/List.h"
#include "../Core/Mutex.h"
#include "../Container/Pair.h"
#include "../Audio/SoundStream.h"

namespace Clockwork
{

/// %Sound stream that supports manual buffering of data from the main thread.
class CLOCKWORK_API BufferedSoundStream : public SoundStream
{
public:
    /// Construct.
    BufferedSoundStream();
    /// Destruct.
    ~BufferedSoundStream();

    /// Produce sound data into destination. Return number of bytes produced. Called by SoundSource from the mixing thread.
    virtual unsigned GetData(signed char* dest, unsigned numBytes);

    /// Buffer sound data. Makes a copy of it.
    void AddData(void* data, unsigned numBytes);
    /// Buffer sound data by taking ownership of it.
    void AddData(SharedArrayPtr<signed char> data, unsigned numBytes);
    /// Buffer sound data by taking ownership of it.
    void AddData(SharedArrayPtr<signed short> data, unsigned numBytes);
    /// Remove all buffered audio data.
    void Clear();

    /// Return amount of buffered (unplayed) sound data in bytes.
    unsigned GetBufferNumBytes() const;
    /// Return length of buffered (unplayed) sound data in seconds.
    float GetBufferLength() const;

private:
    /// Buffers and their sizes.
    List<Pair<SharedArrayPtr<signed char>, unsigned> > buffers_;
    /// Byte position in the frontmost buffer.
    unsigned position_;
    /// Mutex for buffer data.
    mutable Mutex bufferMutex_;
};

}

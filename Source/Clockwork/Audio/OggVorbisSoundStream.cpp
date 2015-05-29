#include "../Audio/OggVorbisSoundStream.h"
#include "../Audio/Sound.h"

#include <STB/stb_vorbis.h>

#include "../DebugNew.h"

namespace Clockwork
{

OggVorbisSoundStream::OggVorbisSoundStream(const Sound* sound)
{
    assert(sound && sound->IsCompressed());

    SetFormat(sound->GetIntFrequency(), sound->IsSixteenBit(), sound->IsStereo());
    // If the sound is looped, the stream will automatically rewind at end
    SetStopAtEnd(!sound->IsLooped());

    // Initialize decoder
    data_ = sound->GetData();
    dataSize_ = sound->GetDataSize();
    int error;
    decoder_ = stb_vorbis_open_memory((unsigned char*)data_.Get(), dataSize_, &error, 0);
}

OggVorbisSoundStream::~OggVorbisSoundStream()
{
    // Close decoder
    if (decoder_)
    {
        stb_vorbis* vorbis = static_cast<stb_vorbis*>(decoder_);

        stb_vorbis_close(vorbis);
        decoder_ = 0;
    }
}

unsigned OggVorbisSoundStream::GetData(signed char* dest, unsigned numBytes)
{
    if (!decoder_)
        return 0;

    stb_vorbis* vorbis = static_cast<stb_vorbis*>(decoder_);

    unsigned channels = stereo_ ? 2 : 1;
    unsigned outSamples = stb_vorbis_get_samples_short_interleaved(vorbis, channels, (short*)dest, numBytes >> 1);
    unsigned outBytes = (outSamples * channels) << 1;

    // Rewind and retry if is looping and produced less output than should have
    if (outBytes < numBytes && !stopAtEnd_)
    {
        numBytes -= outBytes;
        stb_vorbis_seek_start(vorbis);
        outSamples = stb_vorbis_get_samples_short_interleaved(vorbis, channels, (short*)(dest + outBytes), numBytes >> 1);
        outBytes += (outSamples * channels) << 1;
    }

    return outBytes;
}

}

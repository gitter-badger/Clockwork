

#include "../Precompiled.h"

#include "../Audio/SoundStream.h"

namespace Clockwork
{

SoundStream::SoundStream() :
    frequency_(44100),
    stopAtEnd_(false),
    sixteenBit_(false),
    stereo_(false)
{
}

SoundStream::~SoundStream()
{
}

void SoundStream::SetFormat(unsigned frequency, bool sixteenBit, bool stereo)
{
    frequency_ = frequency;
    sixteenBit_ = sixteenBit;
    stereo_ = stereo;
}

void SoundStream::SetStopAtEnd(bool enable)
{
    stopAtEnd_ = enable;
}

unsigned SoundStream::GetSampleSize() const
{
    unsigned size = 1;
    if (sixteenBit_)
        size <<= 1;
    if (stereo_)
        size <<= 1;
    return size;
}

};

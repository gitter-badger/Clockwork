#include "../Container/VectorBase.h"

#include "../DebugNew.h"

namespace Clockwork
{

unsigned char* VectorBase::AllocateBuffer(unsigned size)
{
    return new unsigned char[size];
}

}

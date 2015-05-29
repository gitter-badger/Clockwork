#include "../Container/HashBase.h"
#include "../Container/ListBase.h"
#include "../Container/Str.h"
#include "../Container/VectorBase.h"

namespace Clockwork
{

template<> void Swap<String>(String& first, String& second)
{
    first.Swap(second);
}

template<> void Swap<VectorBase>(VectorBase& first, VectorBase& second)
{
    first.Swap(second);
}

template<> void Swap<ListBase>(ListBase& first, ListBase& second)
{
    first.Swap(second);
}

template<> void Swap<HashBase>(HashBase& first, HashBase& second)
{
    first.Swap(second);
}

}

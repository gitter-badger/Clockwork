#pragma once

#include "../../Container/HashMap.h"
#include "../../Graphics/ShaderVariation.h"

namespace Clockwork
{

/// Combined information for specific vertex and pixel shaders.
class ShaderProgram : public RefCounted
{
public:
    /// Construct.
    ShaderProgram(ShaderVariation* vertexShader, ShaderVariation* pixelShader)
    {
        const HashMap<StringHash, ShaderParameter>& vsParams = vertexShader->GetParameters();
        for (HashMap<StringHash, ShaderParameter>::ConstIterator i = vsParams.Begin(); i != vsParams.End(); ++i)
        parameters_[i->first_] = i->second_;

        const HashMap<StringHash, ShaderParameter>& psParams = pixelShader->GetParameters();
        for (HashMap<StringHash, ShaderParameter>::ConstIterator i = psParams.Begin(); i != psParams.End(); ++i)
            parameters_[i->first_] = i->second_;

        // Optimize shader parameter lookup by rehashing to next power of two
        parameters_.Rehash(NextPowerOfTwo(parameters_.Size()));
    }

    /// Combined parameters from the vertex and pixel shader.
    HashMap<StringHash, ShaderParameter> parameters_;
};

}

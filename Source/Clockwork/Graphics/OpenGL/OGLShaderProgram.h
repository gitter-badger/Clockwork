#pragma once

#include "../../Graphics/GPUObject.h"
#include "../../Graphics/GraphicsDefs.h"
#include "../../Container/HashMap.h"
#include "../../Container/RefCounted.h"

namespace Clockwork
{

class ConstantBuffer;
class Graphics;
class ShaderVariation;

/// %Shader parameter definition.
struct ShaderParameter
{
    /// Construct with defaults.
    ShaderParameter() :
        bufferPtr_(0)
    {
    }

    /// Uniform location or byte offset in constant buffer.
    int location_;
    /// Element type.
    unsigned type_;
    /// Constant buffer pointer.
    ConstantBuffer* bufferPtr_;
};

/// Linked shader program on the GPU.
class CLOCKWORK_API ShaderProgram : public RefCounted, public GPUObject
{
public:
    /// Construct.
    ShaderProgram(Graphics* graphics, ShaderVariation* vertexShader, ShaderVariation* pixelShader);
    /// Destruct.
    ~ShaderProgram();

    /// Mark the GPU resource destroyed on context destruction.
    virtual void OnDeviceLost();
    /// Release shader program.
    virtual void Release();

    /// Link the shaders and examine the uniforms and samplers used. Return true if successful.
    bool Link();

    /// Return the vertex shader.
    ShaderVariation* GetVertexShader() const;
    /// Return the pixel shader.
    ShaderVariation* GetPixelShader() const;
    /// Return whether uses a shader parameter.
    bool HasParameter(StringHash param) const;
    /// Return whether uses a texture unit.
    bool HasTextureUnit(TextureUnit unit) const { return useTextureUnit_[unit]; }
    /// Return the info for a shader parameter, or null if does not exist.
    const ShaderParameter* GetParameter(StringHash param) const;
    /// Return linker output.
    const String& GetLinkerOutput() const { return linkerOutput_; }
    /// Return all constant buffers.
    const SharedPtr<ConstantBuffer>* GetConstantBuffers() const { return &constantBuffers_[0]; }

    /// Check whether a shader parameter group needs update. Does not actually check whether parameters exist in the shaders.
    bool NeedParameterUpdate(ShaderParameterGroup group, const void* source);
    /// Clear a parameter source. Affects only the current shader program if appropriate.
    void ClearParameterSource(ShaderParameterGroup group);

    /// Clear all parameter sources from all shader programs by incrementing the global parameter source framenumber.
    static void ClearParameterSources();
    /// Clear a global parameter source when constant buffers change.
    static void ClearGlobalParameterSource(ShaderParameterGroup group);

private:
    /// Vertex shader.
    WeakPtr<ShaderVariation> vertexShader_;
    /// Pixel shader.
    WeakPtr<ShaderVariation> pixelShader_;
    /// Shader parameters.
    HashMap<StringHash, ShaderParameter> shaderParameters_;
    /// Texture unit use.
    bool useTextureUnit_[MAX_TEXTURE_UNITS];
    /// Constant buffers by binding index.
    SharedPtr<ConstantBuffer> constantBuffers_[MAX_SHADER_PARAMETER_GROUPS * 2];
    /// Remembered shader parameter sources for individual uniform mode.
    const void* parameterSources_[MAX_SHADER_PARAMETER_GROUPS];
    /// Shader link error string.
    String linkerOutput_;
    /// Shader parameter source framenumber.
    unsigned frameNumber_;

    /// Global shader parameter source framenumber.
    static unsigned globalFrameNumber;
    /// Remembered global shader parameter sources for constant buffer mode.
    static const void* globalParameterSources[MAX_SHADER_PARAMETER_GROUPS];
};

}
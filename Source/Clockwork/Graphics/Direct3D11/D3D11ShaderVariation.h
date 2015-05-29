#pragma once

#include "../../Graphics/GPUObject.h"
#include "../../Graphics/GraphicsDefs.h"
#include "../../Container/HashSet.h"
#include "../../Container/RefCounted.h"
#include "../../Container/ArrayPtr.h"

namespace Clockwork
{

class ConstantBuffer;
class Shader;

/// %Shader parameter definition.
struct ShaderParameter
{
    /// Construct with defaults.
    ShaderParameter() :
        type_(VS),
        buffer_(0),
        offset_(0),
        size_(0),
        bufferPtr_(0)
    {
    }

    /// Construct with parameters.
    ShaderParameter(ShaderType type, const String& name, unsigned buffer, unsigned offset, unsigned size, ConstantBuffer* ptr = 0) :
        type_(type),
        name_(name),
        buffer_(buffer),
        offset_(offset),
        size_(size),
        bufferPtr_(ptr)
    {
    }

    /// %Shader type.
    ShaderType type_;
    /// Name of the parameter.
    String name_;
    /// Constant buffer index.
    unsigned buffer_;
    /// Offset in constant buffer.
    unsigned offset_;
    /// Size of parameter in bytes.
    unsigned size_;
    /// Constant buffer pointer. Defined only in shader programs.
    ConstantBuffer* bufferPtr_;
};

/// Vertex or pixel shader on the GPU.
class CLOCKWORK_API ShaderVariation : public RefCounted, public GPUObject
{
public:
    /// Construct.
    ShaderVariation(Shader* owner, ShaderType type);
    /// Destruct.
    virtual ~ShaderVariation();

    /// Release the shader.
    virtual void Release();

    /// Compile the shader. Return true if successful.
    bool Create();
    /// Set name.
    void SetName(const String& name);
    /// Set defines.
    void SetDefines(const String& defines);

    /// Return the owner resource.
    Shader* GetOwner() const;
    /// Return shader type.
    ShaderType GetShaderType() const { return type_; }
    /// Return shader name.
    const String& GetName() const { return name_; }
    /// Return full shader name.
    String GetFullName() const { return name_ + "(" + defines_ + ")"; }
    /// Return whether uses a parameter.
    bool HasParameter(StringHash param) const { return parameters_.Contains(param); }
    /// Return whether uses a texture unit (only for pixel shaders.)
    bool HasTextureUnit(TextureUnit unit) const { return useTextureUnit_[unit]; }
    /// Return all parameter definitions.
    const HashMap<StringHash, ShaderParameter>& GetParameters() const { return parameters_; }
    /// Return vertex element mask.
    unsigned GetElementMask() const { return elementMask_; }
    /// Return shader bytecode.
    const PODVector<unsigned char>& GetByteCode() const { return byteCode_; }
    /// Return defines.
    const String& GetDefines() const { return defines_; }
    /// Return compile error/warning string.
    const String& GetCompilerOutput() const { return compilerOutput_; }
    /// Return constant buffer data sizes.
    const unsigned* GetConstantBufferSizes() const { return &constantBufferSizes_[0]; }

private:
    /// Load bytecode from a file. Return true if successful.
    bool LoadByteCode(const String& binaryShaderName);
    /// Compile from source. Return true if successful.
    bool Compile();
    /// Inspect the constant parameters and input layout (if applicable) from the shader bytecode.
    void ParseParameters(unsigned char* bufData, unsigned bufSize);
    /// Save bytecode to a file.
    void SaveByteCode(const String& binaryShaderName);
    /// Calculate constant buffer sizes from parameters.
    void CalculateConstantBufferSizes();

    /// Shader this variation belongs to.
    WeakPtr<Shader> owner_;
    /// Shader type.
    ShaderType type_;
    /// Vertex element mask for vertex shaders. Zero for pixel shaders.
    unsigned elementMask_;
    /// Shader parameters.
    HashMap<StringHash, ShaderParameter> parameters_;
    /// Texture unit use flags.
    bool useTextureUnit_[MAX_TEXTURE_UNITS];
    /// Constant buffer sizes. 0 if a constant buffer slot is not in use.
    unsigned constantBufferSizes_[MAX_SHADER_PARAMETER_GROUPS];
    /// Bytecode. Needed for inspecting the input signature and parameters.
    PODVector<unsigned char> byteCode_;
    /// Shader name.
    String name_;
    /// Defines to use in compiling.
    String defines_;
    /// Shader compile error string.
    String compilerOutput_;
};

}

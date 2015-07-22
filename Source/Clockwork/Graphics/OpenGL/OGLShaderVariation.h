

#pragma once

#include "../../Container/RefCounted.h"
#include "../../Container/ArrayPtr.h"
#include "../../Graphics/GPUObject.h"
#include "../../Graphics/GraphicsDefs.h"

namespace Clockwork
{

class Shader;
class ShaderProgram;

/// Vertex or pixel shader on the GPU.
class CLOCKWORK_API ShaderVariation : public RefCounted, public GPUObject
{
public:
    /// Construct.
    ShaderVariation(Shader* owner, ShaderType type);
    /// Destruct.
    virtual ~ShaderVariation();

    /// Mark the GPU resource destroyed on context destruction.
    virtual void OnDeviceLost();
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

    /// Return name.
    const String& GetName() const { return name_; }

    /// Return defines.
    const String& GetDefines() const { return defines_; }

    /// Return full shader name.
    String GetFullName() const { return name_ + "(" + defines_ + ")"; }

    /// Return compile error/warning string.
    const String& GetCompilerOutput() const { return compilerOutput_; }

private:
    /// Shader this variation belongs to.
    WeakPtr<Shader> owner_;
    /// Shader type.
    ShaderType type_;
    /// Shader name.
    String name_;
    /// Defines to use in compiling.
    String defines_;
    /// Shader compile error string.
    String compilerOutput_;
};

}

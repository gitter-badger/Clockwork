

#pragma once

#include "../Container/ArrayPtr.h"
#include "../Resource/Resource.h"

namespace Clockwork
{

class ShaderVariation;

/// %Shader resource consisting of several shader variations.
class CLOCKWORK_API Shader : public Resource
{
    OBJECT(Shader);

public:
    /// Construct.
    Shader(Context* context);
    /// Destruct.
    virtual ~Shader();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Load resource from stream. May be called from a worker thread. Return true if successful.
    virtual bool BeginLoad(Deserializer& source);
    /// Finish resource loading. Always called from the main thread. Return true if successful.
    virtual bool EndLoad();

    /// Return a variation with defines.
    ShaderVariation* GetVariation(ShaderType type, const String& defines);
    /// Return a variation with defines.
    ShaderVariation* GetVariation(ShaderType type, const char* defines);

    /// Return either vertex or pixel shader source code.
    const String& GetSourceCode(ShaderType type) const { return type == VS ? vsSourceCode_ : psSourceCode_; }

    /// Return the latest timestamp of the shader code and its includes.
    unsigned GetTimeStamp() const { return timeStamp_; }

private:
    /// Process source code and include files. Return true if successful.
    bool ProcessSource(String& code, Deserializer& file);
    /// Sort the defines and strip extra spaces to prevent creation of unnecessary duplicate shader variations.
    String NormalizeDefines(const String& defines);
    /// Recalculate the memory used by the shader.
    void RefreshMemoryUse();

    /// Source code adapted for vertex shader.
    String vsSourceCode_;
    /// Source code adapted for pixel shader.
    String psSourceCode_;
    /// Vertex shader variations.
    HashMap<StringHash, SharedPtr<ShaderVariation> > vsVariations_;
    /// Pixel shader variations.
    HashMap<StringHash, SharedPtr<ShaderVariation> > psVariations_;
    /// Source code timestamp.
    unsigned timeStamp_;
    /// Number of unique variations so far.
    unsigned numVariations_;
};

}

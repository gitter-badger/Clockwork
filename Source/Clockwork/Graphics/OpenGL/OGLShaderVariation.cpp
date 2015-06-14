#include "../../Graphics/Graphics.h"
#include "../../Graphics/GraphicsImpl.h"
#include "../../IO/Log.h"
#include "../../Graphics/Shader.h"
#include "../../Graphics/ShaderProgram.h"
#include "../../Graphics/ShaderVariation.h"

#include "../../DebugNew.h"

namespace Clockwork
{

ShaderVariation::ShaderVariation(Shader* owner, ShaderType type) :
    GPUObject(owner->GetSubsystem<Graphics>()),
    owner_(owner),
    type_(type)
{
}

ShaderVariation::~ShaderVariation()
{
    Release();
}

void ShaderVariation::OnDeviceLost()
{
    GPUObject::OnDeviceLost();

    compilerOutput_.Clear();
}

void ShaderVariation::Release()
{
    if (object_)
    {
        if (!graphics_)
            return;

        if (!graphics_->IsDeviceLost())
        {
            if (type_ == VS)
            {
                if (graphics_->GetVertexShader() == this)
                    graphics_->SetShaders(0, 0);
            }
            else
            {
                if (graphics_->GetPixelShader() == this)
                    graphics_->SetShaders(0, 0);
            }

            glDeleteShader(object_);
        }

        object_ = 0;
        graphics_->CleanupShaderPrograms(this);
    }

    compilerOutput_.Clear();
}

bool ShaderVariation::Create()
{
    Release();

    if (!owner_)
    {
        compilerOutput_ = "Owner shader has expired";
        return false;
    }

    object_ = glCreateShader(type_ == VS ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
    if (!object_)
    {
        compilerOutput_ = "Could not create shader object";
        return false;
    }

    const String& originalShaderCode = owner_->GetSourceCode(type_);
    String shaderCode;

    // Check if the shader code contains a version define
    unsigned verStart = originalShaderCode.Find('#');
    unsigned verEnd = 0;
    if (verStart != String::NPOS)
    {
        if (originalShaderCode.Substring(verStart + 1, 7) == "version")
        {
            verEnd = verStart + 9;
            while (verEnd < originalShaderCode.Length())
            {
                if (IsDigit(originalShaderCode[verEnd]))
                    ++verEnd;
                else
                    break;
            }
            // If version define found, insert it first
            String versionDefine = originalShaderCode.Substring(verStart, verEnd - verStart);
            shaderCode += versionDefine + "\n";
        }
    }
    // Force GLSL version 150 if no version define and GL3 is being used
    if (!verEnd && Graphics::GetGL3Support())
        shaderCode += "#version 150\n";

    // Distinguish between VS and PS compile in case the shader code wants to include/omit different things
    shaderCode += type_ == VS ? "#define COMPILEVS\n" : "#define COMPILEPS\n";

    // Add define for the maximum number of supported bones
    shaderCode += "#define MAXBONES " + String(Graphics::GetMaxBones()) + "\n";

    // Prepend the defines to the shader code
    Vector<String> defineVec = defines_.Split(' ');
    for (unsigned i = 0; i < defineVec.Size(); ++i)
    {
        // Add extra space for the checking code below
        String defineString = "#define " + defineVec[i].Replaced('=', ' ') + " \n";
        shaderCode += defineString;

        // In debug mode, check that all defines are referenced by the shader code
        #ifdef _DEBUG
        String defineCheck = defineString.Substring(8, defineString.Find(' ', 8) - 8);
        if (originalShaderCode.Find(defineCheck) == String::NPOS)
            LOGWARNING("Shader " + GetFullName() + " does not use the define " + defineCheck);
        #endif
    }

    #ifdef RPI
    if (type_ == VS)
        shaderCode += "#define RPI\n";
    #endif
    #ifdef EMSCRIPTEN
    shaderCode += "#define WEBGL\n";
    #endif
    if (Graphics::GetGL3Support())
        shaderCode += "#define GL3\n";

    // When version define found, do not insert it a second time
    if (verEnd > 0)
        shaderCode += (originalShaderCode.CString() + verEnd);
    else
        shaderCode += originalShaderCode;

    const char* shaderCStr = shaderCode.CString();
    glShaderSource(object_, 1, &shaderCStr, 0);
    glCompileShader(object_);

    int compiled, length;
    glGetShaderiv(object_, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        glGetShaderiv(object_, GL_INFO_LOG_LENGTH, &length);
        compilerOutput_.Resize(length);
        int outLength;
        glGetShaderInfoLog(object_, length, &outLength, &compilerOutput_[0]);
        glDeleteShader(object_);
        object_ = 0;
    }
    else
        compilerOutput_.Clear();

    return object_ != 0;
}

void ShaderVariation::SetName(const String& name)
{
    name_ = name;
}

void ShaderVariation::SetDefines(const String& defines)
{
    defines_ = defines;
}

Shader* ShaderVariation::GetOwner() const
{
    return owner_;
}

}
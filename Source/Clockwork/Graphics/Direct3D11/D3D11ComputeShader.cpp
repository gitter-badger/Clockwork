#include "Graphics/ComputeShader.h"
#include "Core/Context.h"
#include "Graphics/ShaderVariation.h"
#include "Graphics/Graphics.h"
#include "Graphics/GraphicsImpl.h"

namespace Clockwork
{

ComputeShader::ComputeShader(Context* context) :
    Shader(context)
{

}

ComputeShader::~ComputeShader()
{

}

void ComputeShader::RegisterObject(Context* context)
{
    context->RegisterFactory<ComputeShader>();
}

void ComputeShader::Execute(ComputeBuffer* computeBuffer)
{
    ShaderVariation* shader = GetVariation(GS, "");
    if (shader && shader->GetGPUObject())
    {
       Graphics* graphics = GetSubsystem<Graphics>();

        graphics->GetImpl()->GetDeviceContext()->CSSetShader((ID3D11ComputeShader*)shader->GetGPUObject(), 0, 0);
        graphics->GetImpl()->GetDeviceContext()->Dispatch(threadsX_, threadsY_, threadsZ_);
    }
}

}

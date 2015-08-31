#include "Core/Context.h"
#include "Graphics/GPUBuffer.h"
#include "Graphics/Graphics.h"
#include "Graphics/GraphicsImpl.h"
#include "IO/Log.h"

namespace Clockwork
{

GPUBuffer::GPUBuffer(Context* context) :
    Object(context),
    GPUObject(GetSubsystem<Graphics>())
{

}

GPUBuffer::~GPUBuffer()
{
    Release();
}

void GPUBuffer::CreateBuffer(GPUBufferType type, unsigned size, unsigned stride)
{
    Graphics* graphics = GetSubsystem<Graphics>();

    const bool needUav = false;
    const bool needSrv = false;
    const bool drawIndirect = false;

    D3D11_BUFFER_DESC bufferDesc;
    if (type == GBT_DEFAULT)
    {
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    }
    else if (type == GBT_DYNAMIC)
    {
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    }
    else if (type == GBT_IMMUTABLE)
    {
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    }
    bufferDesc.BindFlags = (needUav ? D3D11_BIND_UNORDERED_ACCESS : 0) | (needSrv ? D3D11_BIND_SHADER_RESOURCE : 0);

    graphics->GetImpl()->GetDevice()->CreateBuffer(&bufferDesc, 0, (ID3D11Buffer**)&object_);
    if (!object_)
    {
        LOGERROR("Failed to create GPUBuffer");
        return;
    }

    if (needUav)
    {
        D3D11_UNORDERED_ACCESS_VIEW_DESC uavd;
        //uavd.Format = format;
        uavd.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
        uavd.Buffer.FirstElement = 0;
        uavd.Buffer.NumElements = size / stride;
        uavd.Buffer.Flags = 0;
        graphics->GetImpl()->GetDevice()->CreateUnorderedAccessView((ID3D11Buffer*)object_, &uavd, (ID3D11UnorderedAccessView**)&uav_);
        if (!uav_)
        {
            LOGERROR("Failed to create Unordered access view");
        }
    }

    if (needSrv)
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
        //srvd.Format = format;
        srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        srvd.Buffer.FirstElement = 0;
        srvd.Buffer.NumElements = size / stride;
        graphics->GetImpl()->GetDevice()->CreateShaderResourceView((ID3D11Buffer*)object_, &srvd, (ID3D11ShaderResourceView**)&srv_);
        if (!srv_)
        {
            LOGERROR("Failed to create Shader Resource View");
        }
    }
}

void GPUBuffer::Release()
{
    if (object_)
    {
        ((ID3D11Buffer*)object_)->Release();
        object_ = 0;
    }
}

unsigned GPUBuffer::Read(void* dest, unsigned size)
{
    if (caretPos_ + size < data_.Size())
    {
        // todo read
        caretPos_ += size;
    }
    return 0;
}

unsigned GPUBuffer::Seek(unsigned position)
{
    caretPos_ = position;
    return caretPos_;
}

unsigned GPUBuffer::Write(const void* data, unsigned size)
{
    if (caretPos_ + size < data_.Size())
    {
        //todo
        caretPos_ += size;
        return size;
    }
    return 0;
}

} 

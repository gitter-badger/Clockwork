#include "Graphics/ComputeBuffer.h"
#include "Core/Context.h"
#include "Graphics/Graphics.h"
#include "Graphics/GraphicsImpl.h"
#include "IO/Log.h"

namespace Clockwork
{

  ComputeBuffer::ComputeBuffer(Context* context) :
    Object(context),
    GPUObject(GetSubsystem<Graphics>()),
    bufferObject_(0)
  {
  }

  ComputeBuffer::~ComputeBuffer()
  {
      Release();
  }

  void ComputeBuffer::RegisterObject(Context* context)
  {
      //context->RegisterFactory<ComputeBuffer>();
  }

  /// Initialize the GPU buffer
  void ComputeBuffer::CreateBuffer(ComputeBufferType type, ComputeBufferUsage usage, unsigned size)
  {
      Graphics* graphics = GetSubsystem<Graphics>();
      type_ = type;
      usage_ = usage;

      if (bufferObject_ || object_)
          Release();

      D3D11_BUFFER_DESC bufferDesc;
      memset(&bufferDesc, 0, sizeof(bufferDesc));
      if (usage_ == CBU_DEFAULT)
          bufferDesc.Usage = D3D11_USAGE_DEFAULT;
      else if (usage_ == CBU_DYNAMIC)
          bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
      else if (usage == CBU_IMMUTABLE)
          bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;

      if (type_ == CBT_UNORDERED_ACCESS_VIEW)
          bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
      else
          bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

      graphics->GetImpl()->GetDevice()->CreateBuffer(&bufferDesc, 0, (ID3D11Buffer**)&bufferObject_);

      if (type_ == CBT_SRV)
      {
          D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
          memset(&viewDesc, 0, sizeof(viewDesc));
          graphics->GetImpl()->GetDevice()->CreateShaderResourceView((ID3D11Buffer*)bufferObject_, &viewDesc, (ID3D11ShaderResourceView**)&object_);
          if (!object_)
          {
              LOGERROR("Failed to create Shader Resource View");
              Release();
          }
      }
      else if (type_ == CBT_UNORDERED_ACCESS_VIEW)
      {
          D3D11_UNORDERED_ACCESS_VIEW_DESC viewDesc;
          memset(&viewDesc, 0, sizeof(viewDesc));
          graphics->GetImpl()->GetDevice()->CreateUnorderedAccessView((ID3D11Buffer*)bufferObject_, &viewDesc, (ID3D11UnorderedAccessView**)&object_);
          if (!object_)
          {
              LOGERROR("Failed to create Unordered Access View");
              Release();
          }
      }
  }

  /// Release the GPU resources.
  void ComputeBuffer::Release()
  {
      if (object_)
      {
          object_ = 0;
      }

      if (bufferObject_)
      {
          bufferObject_ = 0;
      }
  }

}

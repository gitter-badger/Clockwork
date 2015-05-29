#include "../../Graphics/Graphics.h"
#include "../../Graphics/GraphicsImpl.h"
#include "../../Graphics/ShaderVariation.h"
#include "../../Graphics/VertexBuffer.h"
#include "../../Graphics/VertexDeclaration.h"
#include "../../IO/Log.h"

#include "../../DebugNew.h"

namespace Clockwork
{

VertexDeclaration::VertexDeclaration(Graphics* graphics, ShaderVariation* vertexShader, VertexBuffer** vertexBuffers, unsigned* elementMasks) :
    inputLayout_(0)
{
    PODVector<D3D11_INPUT_ELEMENT_DESC> elementDescs;

    unsigned vbElementMask = 0;

    for (unsigned i = 0; i < MAX_VERTEX_STREAMS; ++i)
    {
        if (vertexBuffers[i] && elementMasks[i])
        {
            for (unsigned j = 0; j < MAX_VERTEX_ELEMENTS; ++j)
            {
                if (elementMasks[i] & (1 << j))
                {
                    D3D11_INPUT_ELEMENT_DESC newDesc;
                    newDesc.SemanticName = VertexBuffer::elementSemantics[j];
                    newDesc.SemanticIndex = VertexBuffer::elementSemanticIndices[j];
                    newDesc.Format = (DXGI_FORMAT)VertexBuffer::elementFormats[j];
                    newDesc.InputSlot = (unsigned)i;
                    newDesc.AlignedByteOffset = vertexBuffers[i]->GetElementOffset((VertexElement)j);
                    newDesc.InputSlotClass = (j >= ELEMENT_INSTANCEMATRIX1 && j <= ELEMENT_INSTANCEMATRIX3) ?
                        D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
                    newDesc.InstanceDataStepRate = (j >= ELEMENT_INSTANCEMATRIX1 && j <= ELEMENT_INSTANCEMATRIX3) ? 1 : 0;
                    elementDescs.Push(newDesc);
                    vbElementMask |= 1 << j;
                }
            }
        }
    }

    if (elementDescs.Empty())
        return;

    ID3D11InputLayout* d3dInputLayout = 0;
    const PODVector<unsigned char>& byteCode = vertexShader->GetByteCode();

    graphics->GetImpl()->GetDevice()->CreateInputLayout(&elementDescs[0], (unsigned)elementDescs.Size(), &byteCode[0],
        byteCode.Size(), &d3dInputLayout);
    if (d3dInputLayout)
        inputLayout_ = d3dInputLayout;
    else
        LOGERRORF("Failed to create input layout for shader %s, missing element mask %d",
            vertexShader->GetFullName().CString(), vertexShader->GetElementMask() & ~vbElementMask);
}

VertexDeclaration::~VertexDeclaration()
{
    if (inputLayout_)
    {
        ((ID3D11InputLayout*)inputLayout_)->Release();
        inputLayout_ = 0;
    }
}

}

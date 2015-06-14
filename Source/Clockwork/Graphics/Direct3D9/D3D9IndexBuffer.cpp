#include "../../Core/Context.h"
#include "../../Graphics/Graphics.h"
#include "../../Graphics/GraphicsImpl.h"
#include "../../Graphics/IndexBuffer.h"
#include "../../IO/Log.h"

#include "../../DebugNew.h"

namespace Clockwork
{

IndexBuffer::IndexBuffer(Context* context) :
    Object(context),
    GPUObject(GetSubsystem<Graphics>()),
    indexCount_(0),
    indexSize_(0),
    pool_(D3DPOOL_MANAGED),
    usage_(0),
    lockState_(LOCK_NONE),
    lockStart_(0),
    lockCount_(0),
    lockScratchData_(0),
    shadowed_(false)
{
    // Force shadowing mode if graphics subsystem does not exist
    if (!graphics_)
        shadowed_ = true;
}

IndexBuffer::~IndexBuffer()
{
    Release();
}

void IndexBuffer::OnDeviceLost()
{
    if (pool_ == D3DPOOL_DEFAULT)
        Release();
}

void IndexBuffer::OnDeviceReset()
{
    if (pool_ == D3DPOOL_DEFAULT || !object_)
    {
        Create();
        dataLost_ = !UpdateToGPU();
    }
    else if (dataPending_)
        dataLost_ = !UpdateToGPU();

    dataPending_ = false;
}

void IndexBuffer::Release()
{
    Unlock();

    if (object_)
    {
        if (!graphics_)
            return;

        if (graphics_->GetIndexBuffer() == this)
            graphics_->SetIndexBuffer(0);

        ((IDirect3DIndexBuffer9*)object_)->Release();
        object_ = 0;
    }
}

void IndexBuffer::SetShadowed(bool enable)
{
    // If no graphics subsystem, can not disable shadowing
    if (!graphics_)
        enable = true;

    if (enable != shadowed_)
    {
        if (enable && indexCount_ && indexSize_)
            shadowData_ = new unsigned char[indexCount_ * indexSize_];
        else
            shadowData_.Reset();

        shadowed_ = enable;
    }
}

bool IndexBuffer::SetSize(unsigned indexCount, bool largeIndices, bool dynamic)
{
    Unlock();

    if (dynamic)
    {
        pool_ = D3DPOOL_DEFAULT;
        usage_ = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;
    }
    else
    {
        pool_ = D3DPOOL_MANAGED;
        usage_ = 0;
    }

    indexCount_ = indexCount;
    indexSize_ = largeIndices ? sizeof(unsigned) : sizeof(unsigned short);

    if (shadowed_ && indexCount_ && indexSize_)
        shadowData_ = new unsigned char[indexCount_ * indexSize_];
    else
        shadowData_.Reset();

    return Create();
}

bool IndexBuffer::SetData(const void* data)
{
    if (!data)
    {
        LOGERROR("Null pointer for index buffer data");
        return false;
    }

    if (!indexSize_)
    {
        LOGERROR("Index size not defined, can not set index buffer data");
        return false;
    }

    if (shadowData_ && data != shadowData_.Get())
        memcpy(shadowData_.Get(), data, indexCount_ * indexSize_);

    if (object_)
    {
        if (graphics_->IsDeviceLost())
        {
            LOGWARNING("Index buffer data assignment while device is lost");
            dataPending_ = true;
            return true;
        }

        void* hwData = MapBuffer(0, indexCount_, true);
        if (hwData)
        {
            memcpy(hwData, data, indexCount_ * indexSize_);
            UnmapBuffer();
        }
        else
            return false;
    }

    dataLost_ = false;
    return true;
}

bool IndexBuffer::SetDataRange(const void* data, unsigned start, unsigned count, bool discard)
{
    if (start == 0 && count == indexCount_)
        return SetData(data);

    if (!data)
    {
        LOGERROR("Null pointer for index buffer data");
        return false;
    }

    if (!indexSize_)
    {
        LOGERROR("Index size not defined, can not set index buffer data");
        return false;
    }

    if (start + count > indexCount_)
    {
        LOGERROR("Illegal range for setting new index buffer data");
        return false;
    }

    if (!count)
        return true;

    if (shadowData_ && shadowData_.Get() + start * indexSize_ != data)
        memcpy(shadowData_.Get() + start * indexSize_, data, count * indexSize_);

    if (object_)
    {
        if (graphics_->IsDeviceLost())
        {
            LOGWARNING("Index buffer data assignment while device is lost");
            dataPending_ = true;
            return true;
        }

        void* hwData = MapBuffer(start, count, discard);
        if (hwData)
        {
            memcpy(hwData, data, count * indexSize_);
            UnmapBuffer();
        }
        else
            return false;
    }

    return true;
}

void* IndexBuffer::Lock(unsigned start, unsigned count, bool discard)
{
    if (lockState_ != LOCK_NONE)
    {
        LOGERROR("Index buffer already locked");
        return 0;
    }

    if (!indexSize_)
    {
        LOGERROR("Index size not defined, can not lock index buffer");
        return 0;
    }

    if (start + count > indexCount_)
    {
        LOGERROR("Illegal range for locking index buffer");
        return 0;
    }

    if (!count)
        return 0;

    lockStart_ = start;
    lockCount_ = count;

    // Because shadow data must be kept in sync, can only lock hardware buffer if not shadowed
    if (object_ && !shadowData_ && !graphics_->IsDeviceLost())
        return MapBuffer(start, count, discard);
    else if (shadowData_)
    {
        lockState_ = LOCK_SHADOW;
        return shadowData_.Get() + start * indexSize_;
    }
    else if (graphics_)
    {
        lockState_ = LOCK_SCRATCH;
        lockScratchData_ = graphics_->ReserveScratchBuffer(count * indexSize_);
        return lockScratchData_;
    }
    else
        return 0;
}

void IndexBuffer::Unlock()
{
    switch (lockState_)
    {
    case LOCK_HARDWARE:
        UnmapBuffer();
        break;

    case LOCK_SHADOW:
        SetDataRange(shadowData_.Get() + lockStart_ * indexSize_, lockStart_, lockCount_);
        lockState_ = LOCK_NONE;
        break;

    case LOCK_SCRATCH:
        SetDataRange(lockScratchData_, lockStart_, lockCount_);
        if (graphics_)
            graphics_->FreeScratchBuffer(lockScratchData_);
        lockScratchData_ = 0;
        lockState_ = LOCK_NONE;
        break;
    }
}

bool IndexBuffer::IsDynamic() const
{
    return pool_ == D3DPOOL_DEFAULT;
}

bool IndexBuffer::GetUsedVertexRange(unsigned start, unsigned count, unsigned& minVertex, unsigned& vertexCount)
{
    if (!shadowData_)
    {
        LOGERROR("Used vertex range can only be queried from an index buffer with shadow data");
        return false;
    }

    if (start + count > indexCount_)
    {
        LOGERROR("Illegal index range for querying used vertices");
        return false;
    }

    minVertex = M_MAX_UNSIGNED;
    unsigned maxVertex = 0;

    if (indexSize_ == sizeof(unsigned))
    {
        unsigned* indices = ((unsigned*)shadowData_.Get()) + start;

        for (unsigned i = 0; i < count; ++i)
        {
            if (indices[i] < minVertex)
                minVertex = indices[i];
            if (indices[i] > maxVertex)
                maxVertex = indices[i];
        }
    }
    else
    {
        unsigned short* indices = ((unsigned short*)shadowData_.Get()) + start;

        for (unsigned i = 0; i < count; ++i)
        {
            if (indices[i] < minVertex)
                minVertex = indices[i];
            if (indices[i] > maxVertex)
                maxVertex = indices[i];
        }
    }

    vertexCount = maxVertex - minVertex + 1;
    return true;
}

bool IndexBuffer::Create()
{
    Release();

    if (!indexCount_)
        return true;

    if (graphics_)
    {
        if (graphics_->IsDeviceLost())
        {
            LOGWARNING("Index buffer creation while device is lost");
            return true;
        }

        IDirect3DDevice9* device = graphics_->GetImpl()->GetDevice();
        if (!device || FAILED(device->CreateIndexBuffer(
            indexCount_ * indexSize_,
            usage_,
            indexSize_ == sizeof(unsigned) ? D3DFMT_INDEX32 : D3DFMT_INDEX16,
            (D3DPOOL)pool_,
            (IDirect3DIndexBuffer9**)&object_,
            0)))
        {
            LOGERROR("Could not create index buffer");
            return false;
        }
    }

    return true;
}

bool IndexBuffer::UpdateToGPU()
{
    if (object_ && shadowData_)
        return SetData(shadowData_.Get());
    else
        return false;
}

void* IndexBuffer::MapBuffer(unsigned start, unsigned count, bool discard)
{
    void* hwData = 0;

    if (object_)
    {
        DWORD flags = 0;

        if (discard && usage_ & D3DUSAGE_DYNAMIC)
            flags = D3DLOCK_DISCARD;

        if (FAILED(((IDirect3DIndexBuffer9*)object_)->Lock(start * indexSize_, count * indexSize_, &hwData, flags)))
            LOGERROR("Could not lock index buffer");
        else
            lockState_ = LOCK_HARDWARE;
    }

    return hwData;
}

void IndexBuffer::UnmapBuffer()
{
    if (object_ && lockState_ == LOCK_HARDWARE)
    {
        ((IDirect3DIndexBuffer9*)object_)->Unlock();
        lockState_ = LOCK_NONE;
    }
}

}
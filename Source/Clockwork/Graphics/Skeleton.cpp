

#include "../Precompiled.h"

#include "../Graphics/Skeleton.h"
#include "../IO/Log.h"

#include "../DebugNew.h"

namespace Clockwork
{

Skeleton::Skeleton() :
    rootBoneIndex_(M_MAX_UNSIGNED)
{
}

Skeleton::~Skeleton()
{
}

bool Skeleton::Load(Deserializer& source)
{
    ClearBones();

    if (source.IsEof())
        return false;

    unsigned bones = source.ReadUInt();
    bones_.Reserve(bones);

    for (unsigned i = 0; i < bones; ++i)
    {
        Bone newBone;
        newBone.name_ = source.ReadString();
        newBone.nameHash_ = newBone.name_;
        newBone.parentIndex_ = source.ReadUInt();
        newBone.initialPosition_ = source.ReadVector3();
        newBone.initialRotation_ = source.ReadQuaternion();
        newBone.initialScale_ = source.ReadVector3();
        source.Read(&newBone.offsetMatrix_.m00_, sizeof(Matrix3x4));

        // Read bone collision data
        newBone.collisionMask_ = source.ReadUByte();
        if (newBone.collisionMask_ & BONECOLLISION_SPHERE)
            newBone.radius_ = source.ReadFloat();
        if (newBone.collisionMask_ & BONECOLLISION_BOX)
            newBone.boundingBox_ = source.ReadBoundingBox();

        if (newBone.parentIndex_ == i)
            rootBoneIndex_ = i;

        bones_.Push(newBone);
    }

    return true;
}

bool Skeleton::Save(Serializer& dest) const
{
    if (!dest.WriteUInt(bones_.Size()))
        return false;

    for (unsigned i = 0; i < bones_.Size(); ++i)
    {
        const Bone& bone = bones_[i];
        dest.WriteString(bone.name_);
        dest.WriteUInt(bone.parentIndex_);
        dest.WriteVector3(bone.initialPosition_);
        dest.WriteQuaternion(bone.initialRotation_);
        dest.WriteVector3(bone.initialScale_);
        dest.Write(bone.offsetMatrix_.Data(), sizeof(Matrix3x4));

        // Collision info
        dest.WriteUByte(bone.collisionMask_);
        if (bone.collisionMask_ & BONECOLLISION_SPHERE)
            dest.WriteFloat(bone.radius_);
        if (bone.collisionMask_ & BONECOLLISION_BOX)
            dest.WriteBoundingBox(bone.boundingBox_);
    }

    return true;
}

void Skeleton::Define(const Skeleton& src)
{
    ClearBones();

    bones_ = src.bones_;
    // Make sure we clear node references, if they exist
    // (AnimatedModel will create new nodes on its own)
    for (Vector<Bone>::Iterator i = bones_.Begin(); i != bones_.End(); ++i)
        i->node_.Reset();
    rootBoneIndex_ = src.rootBoneIndex_;
}

void Skeleton::SetRootBoneIndex(unsigned index)
{
    if (index < bones_.Size())
        rootBoneIndex_ = index;
    else
        LOGERROR("Root bone index out of bounds");
}

void Skeleton::ClearBones()
{
    bones_.Clear();
    rootBoneIndex_ = M_MAX_UNSIGNED;
}

void Skeleton::Reset()
{
    for (Vector<Bone>::Iterator i = bones_.Begin(); i != bones_.End(); ++i)
    {
        if (i->animated_ && i->node_)
            i->node_->SetTransform(i->initialPosition_, i->initialRotation_, i->initialScale_);
    }
}

void Skeleton::ResetSilent()
{
    for (Vector<Bone>::Iterator i = bones_.Begin(); i != bones_.End(); ++i)
    {
        if (i->animated_ && i->node_)
            i->node_->SetTransformSilent(i->initialPosition_, i->initialRotation_, i->initialScale_);
    }
}


Bone* Skeleton::GetRootBone()
{
    return GetBone(rootBoneIndex_);
}

Bone* Skeleton::GetBone(unsigned index)
{
    return index < bones_.Size() ? &bones_[index] : (Bone*)0;
}

Bone* Skeleton::GetBone(const String& name)
{
    return GetBone(StringHash(name));
}

Bone* Skeleton::GetBone(const char* name)
{
    return GetBone(StringHash(name));
}

Bone* Skeleton::GetBone(StringHash nameHash)
{
    for (Vector<Bone>::Iterator i = bones_.Begin(); i != bones_.End(); ++i)
    {
        if (i->nameHash_ == nameHash)
            return &(*i);
    }

    return 0;
}

}

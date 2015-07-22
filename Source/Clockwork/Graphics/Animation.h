

#pragma once

#include "../Container/Ptr.h"
#include "../Math/Quaternion.h"
#include "../Math/Vector3.h"
#include "../Resource/Resource.h"

namespace Clockwork
{

/// Skeletal animation keyframe.
struct AnimationKeyFrame
{
    /// Keyframe time.
    float time_;
    /// Bone position.
    Vector3 position_;
    /// Bone rotation.
    Quaternion rotation_;
    /// Bone scale.
    Vector3 scale_;
};

/// Skeletal animation track, stores keyframes of a single bone.
struct AnimationTrack
{
    /// Return keyframe index based on time and previous index.
    void GetKeyFrameIndex(float time, unsigned& index) const;

    /// Bone name.
    String name_;
    /// Bone name hash.
    StringHash nameHash_;
    /// Bitmask of included data (position, rotation, scale.)
    unsigned char channelMask_;
    /// Keyframes.
    Vector<AnimationKeyFrame> keyFrames_;
};

/// %Animation trigger point.
struct AnimationTriggerPoint
{
    /// Construct.
    AnimationTriggerPoint() :
        time_(0.0f)
    {
    }

    /// Trigger time.
    float time_;
    /// Trigger data.
    Variant data_;
};

static const unsigned char CHANNEL_POSITION = 0x1;
static const unsigned char CHANNEL_ROTATION = 0x2;
static const unsigned char CHANNEL_SCALE = 0x4;

/// Skeletal animation resource.
class CLOCKWORK_API Animation : public Resource
{
    OBJECT(Animation);

public:
    /// Construct.
    Animation(Context* context);
    /// Destruct.
    virtual ~Animation();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Load resource from stream. May be called from a worker thread. Return true if successful.
    virtual bool BeginLoad(Deserializer& source);
    /// Save resource. Return true if successful.
    virtual bool Save(Serializer& dest) const;

    /// Set animation name.
    void SetAnimationName(const String& name);
    /// Set animation length.
    void SetLength(float length);
    /// Set all animation tracks.
    void SetTracks(const Vector<AnimationTrack>& tracks);
    /// Add a trigger point.
    void AddTrigger(float time, bool timeIsNormalized, const Variant& data);
    /// Remove a trigger point by index.
    void RemoveTrigger(unsigned index);
    /// Remove all trigger points.
    void RemoveAllTriggers();
    /// Resize trigger point vector.
    void SetNumTriggers(unsigned num);

    /// Return animation name.
    const String& GetAnimationName() const { return animationName_; }

    /// Return animation name hash.
    StringHash GetAnimationNameHash() const { return animationNameHash_; }

    /// Return animation length.
    float GetLength() const { return length_; }

    /// Return all animation tracks.
    const Vector<AnimationTrack>& GetTracks() const { return tracks_; }

    /// Return number of animation tracks.
    unsigned GetNumTracks() const { return tracks_.Size(); }

    /// Return animation track by index.
    const AnimationTrack* GetTrack(unsigned index) const;
    /// Return animation track by bone name.
    const AnimationTrack* GetTrack(const String& name) const;
    /// Return animation track by bone name hash.
    const AnimationTrack* GetTrack(StringHash nameHash) const;

    /// Return animation trigger points.
    const Vector<AnimationTriggerPoint>& GetTriggers() const { return triggers_; }

    /// Return number of animation trigger points.
    unsigned GetNumTriggers() const { return triggers_.Size(); }

private:
    /// Animation name.
    String animationName_;
    /// Animation name hash.
    StringHash animationNameHash_;
    /// Animation length.
    float length_;
    /// Animation tracks.
    Vector<AnimationTrack> tracks_;
    /// Animation trigger points.
    Vector<AnimationTriggerPoint> triggers_;
};

}

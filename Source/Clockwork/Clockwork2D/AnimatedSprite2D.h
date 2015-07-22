

#pragma once

#include "../Clockwork2D/Animation2D.h"
#include "../Clockwork2D/StaticSprite2D.h"

/// Loop mode.
enum LoopMode2D
{
    /// Default, use animation's value.
    LM_DEFAULT = 0,
    /// Force looped.
    LM_FORCE_LOOPED,
    /// Force clamped.
    LM_FORCE_CLAMPED
};

namespace Clockwork
{

class AnimationSet2D;

/// Animated sprite component, it uses to play animation created by Spriter (http://www.brashmonkey.com/).
class CLOCKWORK_API AnimatedSprite2D : public StaticSprite2D
{
    OBJECT(AnimatedSprite2D);

public:
    /// Construct.
    AnimatedSprite2D(Context* context);
    /// Destruct.
    virtual ~AnimatedSprite2D();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Handle enabled/disabled state change.
    virtual void OnSetEnabled();

    /// Set speed.
    void SetSpeed(float speed);
    /// Set animation by animation set, name and loop mode.
    void SetAnimation(AnimationSet2D* animationSet, const String& name, LoopMode2D loopMode = LM_DEFAULT);
    /// Set animation by name and loop mode.
    void SetAnimation(const String& name, LoopMode2D loopMode = LM_DEFAULT);
    /// Set animation set.
    void SetAnimationSet(AnimationSet2D* animationSet);
    /// Set loop mode.
    void SetLoopMode(LoopMode2D loopMode);

    /// Return speed.
    float GetSpeed() const { return speed_; }

    /// Return animation name.
    const String& GetAnimation() const { return animationName_; }

    /// Return animation.
    AnimationSet2D* GetAnimationSet() const;

    /// Return loop mode.
    LoopMode2D GetLoopMode() const { return loopMode_; }

    /// Return root node.
    Node* GetRootNode() const;

    /// Set animation set attribute.
    void SetAnimationSetAttr(const ResourceRef& value);
    /// Return animation set attribute.
    ResourceRef GetAnimationSetAttr() const;
    /// Set animation by name.
    void SetAnimationAttr(const String& name);

protected:
    /// Handle scene being assigned.
    virtual void OnSceneSet(Scene* scene);
    /// Recalculate the world-space bounding box.
    virtual void OnWorldBoundingBoxUpdate();
    /// Handle draw order changed.
    virtual void OnDrawOrderChanged();
    /// Handle update vertices.
    virtual void UpdateSourceBatches();
    /// Handle flip changed.
    virtual void OnFlipChanged();
    /// Set animation.
    void SetAnimation(Animation2D* animation, LoopMode2D loopMode);
    /// Update animation.
    void UpdateAnimation(float timeStep);
    /// Calculate time line world world transform.
    void CalculateTimelineWorldTransform(int index);
    /// Handle scene post update.
    void HandleScenePostUpdate(StringHash eventType, VariantMap& eventData);

    /// Speed.
    float speed_;
    /// Animation set.
    SharedPtr<AnimationSet2D> animationSet_;
    /// Animation name.
    String animationName_;
    /// Animation.
    SharedPtr<Animation2D> animation_;
    /// Loop mode.
    LoopMode2D loopMode_;
    /// Looped.
    bool looped_;
    /// Current time.
    float currentTime_;
    /// Root node.
    SharedPtr<Node> rootNode_;
    /// Number of tracks.
    unsigned numTracks_;
    /// Track nodes.
    Vector<SharedPtr<Node> > trackNodes_;
    /// Track node info.
    struct TrackNodeInfo
    {
        /// Has sprite.
        bool hasSprite;
        /// World space.
        bool worldSpace;
        /// Current value.
        AnimationKeyFrame2D value;
    };
    /// Track node infos.
    Vector<TrackNodeInfo> trackNodeInfos_;
};

}

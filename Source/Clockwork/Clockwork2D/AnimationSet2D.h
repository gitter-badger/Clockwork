#pragma once

#include "../Resource/Resource.h"

namespace Clockwork
{

class Animation2D;
class Sprite2D;
class XMLElement;
class XMLFile;

/// Spriter animation set, it includes one or more animations, for more information please refer to http://www.brashmonkey.com/spriter.htm.
class CLOCKWORK_API AnimationSet2D : public Resource
{
    OBJECT(AnimationSet2D);

public:
    /// Construct.
    AnimationSet2D(Context* context);
    /// Destruct.
    virtual ~AnimationSet2D();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Load resource from stream. May be called from a worker thread. Return true if successful.
    virtual bool BeginLoad(Deserializer& source);
    /// Finish resource loading. Always called from the main thread. Return true if successful.
    virtual bool EndLoad();

    /// Get number of animations.
    unsigned GetNumAnimations() const;
    /// Return animation by index.
    Animation2D* GetAnimation(unsigned index) const;
    /// Return animation by name.
    Animation2D* GetAnimation(const String& name) const;

private:
    /// Return sprite by hash.
    Sprite2D* GetSprite(const StringHash& hash) const;
    /// Begin load scml.
    bool BeginLoadSpriter(Deserializer &source);
    /// Finish load scml.
    bool EndLoadSpriter();
    /// Load spriter folders.
    bool LoadSpriterFolders(const XMLElement& rootElem);
    /// Load spriter animation.
    bool LoadSpriterAnimation(const XMLElement& animationElem);

    /// Sprites.
    HashMap<StringHash, SharedPtr<Sprite2D> > sprites_;
    /// Animations.
    Vector<SharedPtr<Animation2D> > animations_;
    /// Spriter file.
    SharedPtr<XMLFile> spriterFile_;
};

}



#pragma once

#include "../Scene/Component.h"

namespace Clockwork
{

/// Component which tags geometry for inclusion in the navigation mesh. Optionally auto-includes geometry from child nodes.
class CLOCKWORK_API Navigable : public Component
{
    OBJECT(Navigable);

public:
    /// Construct.
    Navigable(Context* context);
    /// Destruct.
    virtual ~Navigable();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Set whether geometry is automatically collected from child nodes. Default true.
    void SetRecursive(bool enable);
    /// Return whether geometry is automatically collected from child nodes.
    bool IsRecursive() const { return recursive_; }

private:
    /// Recursive flag.
    bool recursive_;
};

}

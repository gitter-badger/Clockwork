

#pragma once

#include "../Scene/Component.h"

namespace Clockwork
{

/// %Network interest management settings component.
class CLOCKWORK_API NetworkPriority : public Component
{
    OBJECT(NetworkPriority);

public:
    /// Construct.
    NetworkPriority(Context* context);
    /// Destruct.
    virtual ~NetworkPriority();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Set base priority. Default 100 (send updates at full frequency.)
    void SetBasePriority(float priority);
    /// Set priority reduction distance factor. Default 0 (no effect.)
    void SetDistanceFactor(float factor);
    /// Set minimum priority. Default 0 (no updates when far away enough.)
    void SetMinPriority(float priority);
    /// Set whether updates to owner should be sent always at full rate. Default true.
    void SetAlwaysUpdateOwner(bool enable);

    /// Return base priority.
    float GetBasePriority() const { return basePriority_; }

    /// Return priority reduction distance factor.
    float GetDistanceFactor() const { return distanceFactor_; }

    /// Return minimum priority.
    float GetMinPriority() const { return minPriority_; }

    /// Return whether updates to owner should be sent always at full rate.
    bool GetAlwaysUpdateOwner() const { return alwaysUpdateOwner_; }

    /// Increment and check priority accumulator. Return true if should update. Called by Connection.
    bool CheckUpdate(float distance, float& accumulator);

private:
    /// Base priority.
    float basePriority_;
    /// Priority reduction distance factor.
    float distanceFactor_;
    /// Minimum priority.
    float minPriority_;
    /// Update owner at full rate flag.
    bool alwaysUpdateOwner_;
};

}

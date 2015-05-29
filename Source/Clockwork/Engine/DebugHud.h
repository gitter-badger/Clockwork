#pragma once

#include "../Core/Object.h"
#include "../Core/Timer.h"

namespace Clockwork
{

class Engine;
class Font;
class Text;
class XMLFile;

static const unsigned DEBUGHUD_SHOW_NONE = 0x0;
static const unsigned DEBUGHUD_SHOW_STATS = 0x1;
static const unsigned DEBUGHUD_SHOW_MODE = 0x2;
static const unsigned DEBUGHUD_SHOW_PROFILER = 0x4;
static const unsigned DEBUGHUD_SHOW_ALL = 0x7;

/// Displays rendering stats and profiling information.
class CLOCKWORK_API DebugHud : public Object
{
    OBJECT(DebugHud);

public:
    /// Construct.
    DebugHud(Context* context);
    /// Destruct.
    ~DebugHud();

    /// Update. Called by HandlePostUpdate().
    void Update();
    /// Set UI elements' style from an XML file.
    void SetDefaultStyle(XMLFile* style);
    /// Set elements to show.
    void SetMode(unsigned mode);
    /// Set maximum profiler block depth, default unlimited.
    void SetProfilerMaxDepth(unsigned depth);
    /// Set profiler accumulation interval in seconds.
    void SetProfilerInterval(float interval);
    /// Set whether to show 3D geometry primitive/batch count only. Default false.
    void SetUseRendererStats(bool enable);
    /// Toggle elements.
    void Toggle(unsigned mode);
    /// Toggle all elements.
    void ToggleAll();

    /// Return the UI style file.
    XMLFile* GetDefaultStyle() const;
    /// Return rendering stats text.
    Text* GetStatsText() const { return statsText_; }
    /// Return rendering mode text.
    Text* GetModeText() const { return modeText_; }
    /// Return profiler text.
    Text* GetProfilerText() const { return profilerText_; }
    /// Return currently shown elements.
    unsigned GetMode() const { return mode_; }
    /// Return maximum profiler block depth.
    unsigned GetProfilerMaxDepth() const { return profilerMaxDepth_; }
    /// Return profiler accumulation interval in seconds
    float GetProfilerInterval() const;

    /// Return whether showing 3D geometry primitive/batch count only.
    bool GetUseRendererStats() const { return useRendererStats_; }
    /// Set application-specific stats.
    void SetAppStats(const String& label, const Variant& stats);
    /// Set application-specific stats.
    void SetAppStats(const String& label, const String& stats);
    /// Reset application-specific stats. Return true if it was erased successfully.
    bool ResetAppStats(const String& label);
    /// Clear all application-specific stats.
    void ClearAppStats();

private:
    /// Handle logic post-update event. The HUD texts are updated here.
    void HandlePostUpdate(StringHash eventType, VariantMap& eventData);

    /// Rendering stats text.
    SharedPtr<Text> statsText_;
    /// Rendering mode text.
    SharedPtr<Text> modeText_;
    /// Profiling information text.
    SharedPtr<Text> profilerText_;
    /// Hashmap containing application specific stats.
    HashMap<String, String> appStats_;
    /// Profiler timer.
    Timer profilerTimer_;
    /// Profiler max block depth.
    unsigned profilerMaxDepth_;
    /// Profiler accumulation interval.
    unsigned profilerInterval_;
    /// Show 3D geometry primitive/batch count flag.
    bool useRendererStats_;
    /// Current shown-element mode.
    unsigned mode_;
};

}

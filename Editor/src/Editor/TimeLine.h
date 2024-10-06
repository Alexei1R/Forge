#ifndef TIMELINE_H
#define TIMELINE_H

#include "Forge/Core/Log/Log.h"
#include "imgui.h"
#include "ImSequencer.h"
#include <vector>
#include <string>

struct Timeline
{
    std::string name;
    int frameMin;
    int frameMax;
    int startSlider;
    int endSlider;

    Timeline(const std::string& timelineName, int minFrame, int maxFrame) :
        name(timelineName), frameMin(minFrame), frameMax(maxFrame), startSlider(minFrame),
        endSlider(minFrame + 10)
    {
    }
};

class TimelineManager : public ImSequencer::SequenceInterface
{
public:
    TimelineManager();

    // ImSequencer interface implementation
    virtual int GetFrameMin() const override;
    virtual int GetFrameMax() const override;
    virtual int GetItemCount() const override;
    virtual int GetItemTypeCount() const override;
    virtual const char* GetItemTypeName(int typeIndex) const override;
    virtual const char* GetItemLabel(int index) const override;
    virtual void Get(int index, int** start, int** end, int* type, unsigned int* color) override;
    virtual void Add(int type) override;
    virtual void Del(int index) override;
    virtual void Duplicate(int index) override;

    // Custom functions for managing timelines
    void AddTimeline(const std::string& name, int frameMin, int frameMax);
    void RemoveTimeline(int index);
    size_t GetTimelineCount() const
    {
        return mTimelines.size();
    }
    const std::vector<Timeline>& GetTimelines() const
    {
        return mTimelines;
    }

private:
    std::vector<Timeline> mTimelines;
};

#endif  // TIMELINE_H

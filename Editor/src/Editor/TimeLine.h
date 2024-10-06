//
// Created by toor on 2024-10-06.
//

#ifndef TIMELINE_H
#define TIMELINE_H


#include "Forge/Core/Log/Log.h"
#include "imgui.h"
#include "ImSequencer.h"
#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>

struct TimelineItem
{
    int mType;
    int mFrameStart;
    int mFrameEnd = 20;
    bool mExpanded;
    std::string name;
};

struct TimelineData
{
    std::vector<TimelineItem> items;
    std::string name;
    int frameMin;
    int frameMax;
};

class Timeline : public ImSequencer::SequenceInterface
{
public:
    // Interface with sequencer
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

    Timeline() {}

    // Functions for multiple timelines
    void AddTimeline(const std::string& name, int frameMin, int frameMax);
    void RemoveTimeline(size_t index);
    size_t GetTimelineCount() const
    {
        return mTimelines.size();
    }

    std::vector<TimelineData> GetTimeLines()
    {
        return mTimelines;
    }

    // New functions to work with all timelines
    int GetTotalItemCount() const;
    const TimelineData& GetTimelineData(size_t index) const
    {
        return mTimelines[index];
    }

private:
    std::vector<TimelineData> mTimelines;
    const char* name = "TimeLinePlanet";
};
#endif  // TIMELINE_H

//
// Created by toor on 2024-10-06.
//


#include "TimeLine.h"
#include "Forge/Core/Log/Log.h"

int Timeline::GetFrameMin() const
{
    if (mTimelines.empty())
        return 0;
    return std::min_element(mTimelines.begin(),
                            mTimelines.end(),
                            [](const TimelineData& a, const TimelineData& b) {
                                return a.frameMin < b.frameMin;
                            })
        ->frameMin;
}

int Timeline::GetFrameMax() const
{
    if (mTimelines.empty())
        return 0;
    return std::max_element(mTimelines.begin(),
                            mTimelines.end(),
                            [](const TimelineData& a, const TimelineData& b) {
                                return a.frameMax < b.frameMax;
                            })
        ->frameMax;
}

int Timeline::GetItemCount() const
{
    return GetTotalItemCount();
}

int Timeline::GetTotalItemCount() const
{
    int count = 0;
    for (const auto& timeline : mTimelines)
    {
        count += timeline.items.size();
    }
    return count;
}

int Timeline::GetItemTypeCount() const
{
    return mTimelines.size();
}

const char* Timeline::GetItemTypeName(int typeIndex) const
{
    if (typeIndex >= 0 && typeIndex < mTimelines.size())
    {
        return mTimelines[typeIndex].name.c_str();
    }
    return "Invalid Timeline";
}

const char* Timeline::GetItemLabel(int index) const
{
    static char tmps[512];
    int currentIndex = 0;
    for (const auto& timeline : mTimelines)
    {
        if (index < currentIndex + timeline.items.size())
        {
            int localIndex = index - currentIndex;
            snprintf(tmps,
                     120,
                     "%s",
                     timeline.name.c_str(),
                     localIndex,
                     timeline.items[localIndex].name.c_str());
            return tmps;
        }
        currentIndex += timeline.items.size();
    }
    snprintf(tmps, 512, "Invalid Item");
    return tmps;
}

void Timeline::Get(int index, int** start, int** end, int* type, unsigned int* color)
{
    int currentIndex = 0;
    for (size_t i = 0; i < mTimelines.size(); ++i)
    {
        const auto& timeline = mTimelines[i];
        if (index < currentIndex + timeline.items.size())
        {
            int localIndex = index - currentIndex;
            const TimelineItem& item = timeline.items[localIndex];
            if (start)
                *start = const_cast<int*>(&item.mFrameStart);
            if (end)
                *end = const_cast<int*>(&item.mFrameEnd);
            if (type)
                *type = i;
            if (color)
                *color = 0xFFAA8080 + (i * 0x00110000);
            return;
        }
        currentIndex += timeline.items.size();
    }
}

void Timeline::Add(int type)
{
    if (type >= 0 && type < mTimelines.size())
    {
        auto& timeline = mTimelines[type];
        int totalFrames = timeline.frameMax - timeline.frameMin;
        int defaultItemSize = totalFrames / 10;

        defaultItemSize = std::max(1, defaultItemSize);

        LOG_INFO("Adding item to timeline {}", type);

        timeline.items.push_back(TimelineItem {
            0,
            timeline.frameMin,
            timeline.frameMin + defaultItemSize,
            false,
            timeline.name});
    }
}

void Timeline::Del(int index) {}

void Timeline::Duplicate(int index) {}

void Timeline::AddTimeline(const std::string& name, int frameMin, int frameMax)
{
    TimelineData newTimeline;
    newTimeline.name = name;
    newTimeline.frameMin = frameMin;
    newTimeline.frameMax = frameMax;
    newTimeline.items.push_back(TimelineItem {0, frameMin, frameMin + 1, false, name});

    mTimelines.push_back(newTimeline);
    LOG_INFO("Added new timeline: {}", name);
}

void Timeline::RemoveTimeline(size_t index)
{
    if (index < mTimelines.size())
    {
        mTimelines.erase(mTimelines.begin() + index);
        LOG_INFO("Removed timeline at index: {}", index);
    }
}

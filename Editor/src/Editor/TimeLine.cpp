#include "TimeLine.h"
#include "Forge/Core/Log/Log.h"

TimelineManager::TimelineManager() {}

int TimelineManager::GetFrameMin() const
{
    if (mTimelines.empty())
        return 0;
    return std::min_element(mTimelines.begin(),
                            mTimelines.end(),
                            [](const Timeline& a, const Timeline& b) {
                                return a.frameMin < b.frameMin;
                            })
        ->frameMin;
}

int TimelineManager::GetFrameMax() const
{
    if (mTimelines.empty())
        return 0;
    return std::max_element(mTimelines.begin(),
                            mTimelines.end(),
                            [](const Timeline& a, const Timeline& b) {
                                return a.frameMax < b.frameMax;
                            })
        ->frameMax;
}

int TimelineManager::GetItemCount() const
{
    return static_cast<int>(mTimelines.size());
}

int TimelineManager::GetItemTypeCount() const
{
    return static_cast<int>(mTimelines.size());  // Each timeline can be considered its own type
}

const char* TimelineManager::GetItemTypeName(int typeIndex) const
{
    if (typeIndex >= 0 && typeIndex < mTimelines.size())
    {
        return mTimelines[typeIndex].name.c_str();
    }
    return "Invalid Timeline";
}

const char* TimelineManager::GetItemLabel(int index) const
{
    if (index >= 0 && index < mTimelines.size())
    {
        return mTimelines[index].name.c_str();
    }
    return "Invalid Item";
}

void TimelineManager::Get(int index, int** start, int** end, int* type, unsigned int* color)
{
    if (index >= 0 && index < mTimelines.size())
    {
        const auto& timeline = mTimelines[index];
        if (start)
            *start = const_cast<int*>(&timeline.startSlider);  // Cast away const-ness
        if (end)
            *end = const_cast<int*>(&timeline.endSlider);  // Cast away const-ness
        if (type)
            *type = index;
        if (color)
            *color = 0xFFAA8080 + (index * 0x00110000);  // Simple color scheme
    }
    else
    {
        LOG_ERROR("Invalid timeline index: {}", index);
    }
}

void TimelineManager::Add(int type)
{
    if (type >= 0 && type < mTimelines.size())
    {
        auto& timeline = mTimelines[type];
        int totalFrames = timeline.frameMax - timeline.frameMin;
        int defaultItemSize = std::max(1, totalFrames / 10);
        timeline.startSlider = timeline.frameMin;
        timeline.endSlider = timeline.frameMin + defaultItemSize;
        LOG_INFO("Added item to timeline {}", type);
    }
    else
    {
        LOG_ERROR("Invalid timeline type: {}", type);
    }
}

void TimelineManager::Del(int index)
{
    if (index >= 0 && index < mTimelines.size())
    {
        mTimelines.erase(mTimelines.begin() + index);
        LOG_INFO("Deleted timeline at index: {}", index);
    }
    else
    {
        LOG_ERROR("Invalid timeline index: {}", index);
    }
}

void TimelineManager::Duplicate(int index)
{
    if (index >= 0 && index < mTimelines.size())
    {
        const auto& timeline = mTimelines[index];
        mTimelines.emplace_back(timeline.name, timeline.frameMin, timeline.frameMax);
        LOG_INFO("Duplicated timeline at index: {}", index);
    }
    else
    {
        LOG_ERROR("Invalid timeline index: {}", index);
    }
}

void TimelineManager::AddTimeline(const std::string& name, int frameMin, int frameMax)
{
    mTimelines.emplace_back(name, frameMin, frameMax);
    LOG_INFO("Added new timeline: {}", name);
}

void TimelineManager::RemoveTimeline(int index)
{
    if (index >= 0 && index < mTimelines.size())
    {
        mTimelines.erase(mTimelines.begin() + index);
        LOG_INFO("Removed timeline at index: {}", index);
    }
    else
    {
        LOG_ERROR("Invalid timeline index: {}", index);
    }
}

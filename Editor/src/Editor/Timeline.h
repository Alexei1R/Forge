//
// Created by toor on 2024-10-04.
//

#ifndef TIMELINE_H
#define TIMELINE_H

#include "Forge/Core/Log/Log.h"
#include "imgui.h"
#include "ImSequencer.h"
#include <cstdio>
#include <vector>

// Single entry name for the timeline
static const char* name = "TimeLinePlanet";

struct Timeline : public ImSequencer::SequenceInterface
{
    // Interface with sequencer
    virtual int GetFrameMin() const
    {
        return mFrameMin;
    }
    virtual int GetFrameMax() const
    {
        return mFrameMax;
    }
    virtual int GetItemCount() const
    {
        return 1;  // Limit to one track
    }

    virtual int GetItemTypeCount() const
    {
        return 1;  // Only one type, no need for multiple types
    }

    virtual const char* GetItemTypeName(int typeIndex) const
    {
        return name;  // Return the name of the single entry
    }

    virtual const char* GetItemLabel(int index) const
    {
        static char tmps[512];
        snprintf(tmps, 512, "[%02d] %s", index, name);  // Use the name for labeling
        return tmps;
    }

    virtual void Get(int index, int** start, int** end, int* type, unsigned int* color)
    {
        TimelineItem& item = myItems[index];
        if (color)
        {
            *color = 0xFFAA8080;  // Same color for the track
        }
        if (start)
        {
            *start = &item.mFrameStart;
        }
        if (end)
        {
            *end = &item.mFrameEnd;
        }
        if (type)
        {
            *type = item.mType;
        }
    }

    virtual void Add(int type)
    {
        LOG_INFO("+")
    }

    virtual void Del(int index)
    {
        LOG_INFO("-")
    }

    virtual void Duplicate(int index)
    {
        // No duplication allowed for single track
    }

    // Custom data for the timeline
    Timeline() : mFrameMin(1974), mFrameMax(2024)
    {
        // Add one default track
        myItems.push_back(TimelineItem {0, mFrameMin, mFrameMin + 1, false});
    }

    int mFrameMin, mFrameMax;
    struct TimelineItem
    {
        int mType;
        int mFrameStart, mFrameEnd;
        bool mExpanded;
    };
    std::vector<TimelineItem> myItems;
};


#endif

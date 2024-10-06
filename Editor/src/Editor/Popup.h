//
// Created by toor on 2024-10-05.
//


#ifndef POPUP_H
#define POPUP_H

#include <string>
#include <vector>
#include <unordered_set>
#include "imgui.h"
#include <functional>

struct DropElements
{
    std::string path;
    std::string name;
    std::string extension;
    ImGuiID ID;
};

class DropPopup
{
private:
    using CallbackFN = std::function<void()>;

public:
    void SetData(const std::vector<std::string>& filePaths);
    void DrawPopup();
    void SaveData();

    void OpenNewPopup();

    void SetCloseCallback(CallbackFN func)
    {
        m_EventCallback = func;
    }

    std::vector<std::string> GetSelectedPaths()
    {
        return m_SelectedFilePaths;
    }


private:
    DropElements getDropElements(const std::string& filePath);
    void UpdateLayoutSizes(float avail_width);
    void Draw();

    std::vector<DropElements> m_DropElements;
    bool m_OpenFullscreenPopup = true;
    std::unordered_set<ImGuiID> m_SelectedItems;
    ImGuiID m_LastSelectedItem = 0;

    // Configuration
    float m_IconSize = 40.0f;
    int m_IconSpacing = 0;
    bool m_StretchSpacing = true;

    // Layout calculated sizes
    ImVec2 m_LayoutItemSize;
    ImVec2 m_LayoutItemStep;
    float m_LayoutItemSpacing = 0.0f;
    int m_LayoutColumnCount = 0;
    int m_LayoutLineCount = 0;

    // Helper function to generate unique IDs
    ImGuiID GenerateUniqueID();


    std::vector<std::string> m_SelectedFilePaths;

    CallbackFN m_EventCallback;
};

#endif  // POPUP_H

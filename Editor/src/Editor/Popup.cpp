//
// Created by toor on 2024-10-05.
//

#include "Popup.h"
#include "imgui.h"
#include <cmath>
#include <filesystem>
#include <algorithm>
#include <string>

ImGuiID DropPopup::GenerateUniqueID()
{
    static ImGuiID nextID = 1;
    return nextID++;
}

void DropPopup::SetData(const std::vector<std::string>& filePaths)
{
    m_DropElements.clear();
    for (const auto& filePath : filePaths)
    {
        DropElements element = getDropElements(filePath);
        element.ID = GenerateUniqueID();
        m_DropElements.push_back(element);
    }
}

void DropPopup::OpenNewPopup()
{
    m_OpenFullscreenPopup = true;
}


void DropPopup::DrawPopup()
{
    if (m_OpenFullscreenPopup)
    {
        ImGui::OpenPopup("FullscreenPopup");
        m_OpenFullscreenPopup = false;
    }

    ImVec2 mainWindowPos = ImGui::GetWindowPos();
    ImVec2 mainWindowSize = ImGui::GetWindowSize();
    ImVec2 popupSize = ImVec2(mainWindowSize.x / 1.5, mainWindowSize.y / 2);
    ImVec2 popupPos = ImVec2(mainWindowPos.x + (mainWindowSize.x - popupSize.x) * 0.5f,
                             mainWindowPos.y + (mainWindowSize.y - popupSize.y) * 0.5f);
    ImGui::SetNextWindowPos(popupPos);
    ImGui::SetNextWindowSize(popupSize);

    if (ImGui::BeginPopupModal(
            "FullscreenPopup",
            NULL,
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
    {
        if (!m_DropElements.empty())
        {
            ImGui::Columns(2, "PopupColumns", false);
            ImGui::SetColumnWidth(0, popupSize.x * 0.3f);

            ImGui::BeginChild("ListView");
            static int selected = -1;
            for (int i = 0; i < m_DropElements.size(); i++)
            {
                bool isSelected = (selected == i);
                if (ImGui::Selectable(m_DropElements[i].name.c_str(), isSelected))
                {
                    selected = i;
                }
            }
            ImGui::EndChild();

            ImGui::NextColumn();
            Draw();
            ImGui::Columns(1);
        }
        else
        {
            ImGui::Text("Please import data first");
        }

        if (ImGui::IsKeyPressed(ImGuiKey_Enter))
        {
            SaveData();
            if (m_EventCallback)
            {
                m_EventCallback();
            }
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void DropPopup::SaveData()
{
    m_SelectedFilePaths.clear();
    for (ImGuiID selected_item : m_SelectedItems)
    {
        for (const auto& element : m_DropElements)
        {
            if (element.ID == selected_item)
            {
                m_SelectedFilePaths.push_back(
                    element.path + "/" + element.name + element.extension);
                break;
            }
        }
    }
}

DropElements DropPopup::getDropElements(const std::string& filePath)
{
    std::filesystem::path fsPath(filePath);
    DropElements elements;

    elements.path = fsPath.parent_path().string();
    elements.name = fsPath.stem().string();
    elements.extension = fsPath.extension().string();

    return elements;
}

void DropPopup::UpdateLayoutSizes(float avail_width)
{
    m_LayoutItemSpacing = static_cast<float>(m_IconSpacing);
    if (!m_StretchSpacing)
        avail_width += std::floor(m_LayoutItemSpacing * 0.5f);

    m_LayoutItemSize = ImVec2(std::floor(m_IconSize), std::floor(m_IconSize));
    m_LayoutColumnCount =
        std::max(static_cast<int>(avail_width / (m_LayoutItemSize.x + m_LayoutItemSpacing)), 1);
    m_LayoutLineCount = (m_DropElements.size() + m_LayoutColumnCount - 1) / m_LayoutColumnCount;

    if (m_StretchSpacing && m_LayoutColumnCount > 1)
        m_LayoutItemSpacing = std::floor(avail_width - m_LayoutItemSize.x * m_LayoutColumnCount) /
                              m_LayoutColumnCount;

    m_LayoutItemStep =
        ImVec2(m_LayoutItemSize.x + m_LayoutItemSpacing, m_LayoutItemSize.y + m_LayoutItemSpacing);
}

void DropPopup::Draw()
{
    ImGuiIO& io = ImGui::GetIO();
    float avail_width = ImGui::GetContentRegionAvail().x;
    UpdateLayoutSizes(avail_width);

    ImGui::SetNextWindowContentSize(
        ImVec2(0.0f, m_LayoutLineCount * (m_LayoutItemSize.x + m_LayoutItemSpacing)));
    if (ImGui::BeginChild("Assets", ImVec2(0.0f, -ImGui::GetTextLineHeightWithSpacing()), true))
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 start_pos = ImGui::GetCursorScreenPos();
        start_pos = ImVec2(start_pos.x + m_LayoutItemSpacing * 0.5f,
                           start_pos.y + m_LayoutItemSpacing * 0.5f);

        ImGui::SetCursorScreenPos(start_pos);

        for (int item_idx = 0; item_idx < m_DropElements.size(); ++item_idx)
        {
            DropElements& item_data = m_DropElements[item_idx];
            ImGui::PushID(static_cast<int>(item_data.ID));

            ImVec2 pos =
                ImVec2(start_pos.x + (item_idx % m_LayoutColumnCount) * m_LayoutItemStep.x,
                       start_pos.y + (item_idx / m_LayoutColumnCount) * m_LayoutItemStep.y);
            ImGui::SetCursorScreenPos(pos);

            bool is_selected = m_SelectedItems.find(item_data.ID) != m_SelectedItems.end();
            ImU32 bg_color = is_selected ? ImGui::GetColorU32(ImGuiCol_ButtonActive)
                                         : ImGui::GetColorU32(ImGuiCol_Button);

            ImVec2 box_min(pos.x, pos.y);
            ImVec2 box_max(box_min.x + m_LayoutItemSize.x, box_min.y + m_LayoutItemSize.y);
            draw_list->AddRectFilled(box_min, box_max, bg_color);

            if (is_selected)
            {
                ImU32 outline_color = IM_COL32(255, 165, 0, 255);
                draw_list->AddRect(box_min, box_max, outline_color, 0.0f, 0, 3.0f);
            }
            else
            {
                ImU32 outline_color = IM_COL32(0, 165, 250, 255);
                draw_list->AddRect(box_min, box_max, outline_color, 0.0f, 0, 1.0f);
            }

            char label[32];

            std::string str = item_data.name;
            /*if (str.length() >= 5)*/
            /*{*/
            /*    str = str.substr(str.length() - 5);*/
            /*}*/
            /*else*/
            /*{*/
            /*}*/

            size_t p = str.find("_");

            // Loop until there are no more underscores
            while (p != std::string::npos)
            {
                // Replace the underscore with a newline character
                str.replace(p, 1, "\n");

                // Find the next underscore
                p = str.find("_", p + 1);  // Move to the next position
            }


            snprintf(label, sizeof(label), "%s", str.c_str());
            ImVec2 text_size = ImGui::CalcTextSize(label);
            ImVec2 text_pos = ImVec2(box_min.x + (m_LayoutItemSize.x - text_size.x) * 0.5f,
                                     box_min.y + (m_LayoutItemSize.y - text_size.y) * 0.5f);
            draw_list->AddText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), label);

            if (ImGui::InvisibleButton("ItemButton", m_LayoutItemSize))
            {
                bool ctrl_held = io.KeyCtrl;
                bool shift_held = io.KeyShift;

                if (shift_held && m_LastSelectedItem != 0)
                {
                    int last_idx = -1;
                    int current_idx = item_idx;
                    for (int i = 0; i < m_DropElements.size(); i++)
                    {
                        if (m_DropElements[i].ID == m_LastSelectedItem)
                        {
                            last_idx = i;
                            break;
                        }
                    }

                    if (last_idx != -1)
                    {
                        int min_idx = std::min(last_idx, current_idx);
                        int max_idx = std::max(last_idx, current_idx);

                        m_SelectedItems.clear();
                        for (int i = min_idx; i <= max_idx; i++)
                        {
                            m_SelectedItems.insert(m_DropElements[i].ID);
                        }
                    }
                }
                else if (ctrl_held)
                {
                    auto it = m_SelectedItems.find(item_data.ID);
                    if (it != m_SelectedItems.end())
                        m_SelectedItems.erase(it);
                    else
                        m_SelectedItems.insert(item_data.ID);
                }
                else
                {
                    m_SelectedItems.clear();
                    m_SelectedItems.insert(item_data.ID);
                }

                m_LastSelectedItem = item_data.ID;
            }

            ImGui::PopID();
        }
    }
    ImGui::EndChild();

    // Show the selected items
    if (!m_SelectedItems.empty())
    {
        ImGui::Text("Selected items: ");
        for (ImGuiID selected_item : m_SelectedItems)
        {
            ImGui::SameLine();
            ImGui::Text("%d", selected_item);
        }
    }
    else
    {
        ImGui::Text("No items selected.");
    }
}

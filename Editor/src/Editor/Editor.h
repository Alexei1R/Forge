//
// Created by toor on 2024-09-15.
//

#ifndef EDITOR_H
#define EDITOR_H

#include "Forge.h"
#include "glm/fwd.hpp"
#include "src/Editor/Planet.h"
#include "src/Editor/Popup.h"
#include "src/Editor/TimeLine.h"
#include <memory>


namespace Forge {

struct PlotEntry
{
    std::vector<double> timestamps;
    std::vector<double> values;
};

struct PlotData
{
    std::vector<std::pair<int, int>> csvData;
    bool loaded = false;
    std::string name;

    // Add PlotEntry for storing timestamps and values
    PlotEntry plotEntry;

    PlotData(const std::vector<std::pair<int, int>>& data,
             bool isLoaded,
             const std::string& plotName) : csvData(data), loaded(isLoaded), name(plotName)
    {
    }
};

class Editor : public Component
{
public:
    Editor();
    ~Editor();

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(DeltaTime dt) override;
    void OnEvent(const Event& event) override;
    void OnImGuiRender() override;


private:
    void BeginGUI();
    void EndGUI();
    ImGuiIO* io;
    bool dockspaceOpen = true;
    bool m_ViewportFocused = false, m_ViewportHovered = false;
    glm::vec2 m_ViewportSize = {0.0f, 0.0f};
    glm::vec2 m_ViewportBounds[2];


    std::shared_ptr<Renderer> m_Renderer;
    FramebufferProps m_FramebufferProps;
    std::shared_ptr<Framebuffer> m_Framebuffer;
    std::shared_ptr<OrbitalCamera> m_Camera;


    float m_FrameRate;
    bool m_ReloadShaders = true;

    std::shared_ptr<Planet> space;
    std::vector<std::shared_ptr<Planet>> solarSystem;
    std::vector<PlanetSettings> solarSystemSettings;

    int currentSelectedPlanet;

    TimelineManager timelineManager;
    /*Timeline m_Timeline;*/
    float x_data[1000], y_data[1000];

    DropPopup m_DropPopup;


    std::vector<PlotData> m_PlotData;
    int selectedEntry = -1;
    std::mutex plotDataMutex;
};

}  // namespace Forge


#endif

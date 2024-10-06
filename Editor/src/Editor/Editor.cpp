//
// Created by toor on 2024-09-15.
//

#include "Editor.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Events/Event.h"
#include "Forge/Events/ImplEvent.h"
#include "Forge/Events/KeyCodes.h"
#include "Forge/Renderer/Buffer.h"
#include "Forge/Renderer/Shader.h"
#include "glm/fwd.hpp"
#include "imgui.h"
#include <future>
#include <memory>

#include "implot/implot.h"


namespace Forge {


Editor::Editor() : Component(ComponentType::None), currentSelectedPlanet(0) {}

Editor::~Editor() {}

void Editor::OnAttach()
{
    m_Framebuffer = std::make_shared<Framebuffer>(m_FramebufferProps);
    m_Renderer = std::make_shared<Renderer>();
    m_Renderer->SetClearColor(glm::vec3(0.3, 0.35, 0.4));

    m_Camera = std::make_shared<OrbitalCamera>(glm::vec3(0.0f, 0.0f, 0.0f), 1920, 1080);

    m_Camera->SetCameraTarget(glm::vec3(30.0f, 0.0f, 0.0f));
    Forge::GetForgeInstance().PushComponent(m_Camera);


    solarSystemSettings.push_back(
        PlanetSettings("Sun", "Assets/Textures/sun.png", glm::vec3(0.0f), 5.0f));
    solarSystemSettings.push_back(
        PlanetSettings("Earth", "Assets/Textures/earth.png", glm::vec3(30.0f, 0.0f, 0.0f), 1.0f));
    solarSystemSettings.push_back(
        PlanetSettings("Mars", "Assets/Textures/martie.png", glm::vec3(20.0f, 0.0f, 20.0f), 0.7f));


    PlanetSettings spaceSettings(
        "Space",
        "Assets/Textures/way.png",
        glm::vec3(30.0f, 0.0f, 0.0f),
        100);
    space = std::make_shared<Planet>(spaceSettings);

    // Create the planets and add them to the solar system vector
    for (const auto& planetSettings : solarSystemSettings)
    {
        auto planet = std::make_shared<Planet>(planetSettings);
        solarSystem.push_back(planet);
    }


    for (int i = 0; i < 1000; i++)
    {
        x_data[i] = i * 0.01;  // Scale x values appropriately for sine wave
        y_data[i] = sin(x_data[i]);  // Sine wave for y values
    }


    m_DropPopup.SetCloseCallback([this]() {
        for (auto data : m_DropPopup.GetSelectedPaths())
        {
            timelineManager.AddTimeline(data, 0, 144);
            m_PlotData.push_back(PlotData({}, false, data));
        }
    });

    ImGuiIO& io = ImGui::GetIO();
    ImFont* times = io.Fonts->AddFontFromFileTTF("Assets/Fonts/times.ttf", 14.0f);
    ImFont* rostov = io.Fonts->AddFontFromFileTTF("Assets/Fonts/rostov.ttf", 64.0f);
}

void Editor::OnDetach() {}
void Editor::OnUpdate(DeltaTime dt)
{
    // Hovered Viewport
    m_Camera->AllowRotation(m_ViewportHovered);
    m_Camera->AllowScroll(m_ViewportHovered);

    m_FrameRate = 1 / dt;
    // Resize
    if (FramebufferProps props = m_Framebuffer->GetSpecification();
        m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&  // zero sized framebuffer is invalid
        (props.width != m_ViewportSize.x || props.height != m_ViewportSize.y))
    {
        m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_Camera->Resize(m_ViewportSize.x, m_ViewportSize.y);
    }


    if (m_ReloadShaders)
    {
        m_ReloadShaders = false;
    }


    m_Framebuffer->Bind();
    m_Renderer->Clear();
    space->Draw(m_Camera);

    for (size_t i = 0; i < solarSystem.size(); ++i)
    {
        // Draw the planet with its settings
        solarSystem[i]->Draw(m_Camera);
    }


    m_Framebuffer->UnBind();
}


void Editor::OnEvent(const Event& event)
{
    if (event.GetType() == EventType::Key)
    {
        KeyEvent keyEv = static_cast<const KeyEvent&>(event);
        if (keyEv.GetAction() == Action::KeyPress)
        {
            if (keyEv.GetKey() == Key::S)
            {
                m_ReloadShaders = true;
            }

            if (keyEv.GetKey() == Key::Q)
            {
                currentSelectedPlanet--;
                if (currentSelectedPlanet <= 0)
                {
                    currentSelectedPlanet = 0;
                }

                m_Camera->SetCameraTarget(solarSystemSettings[currentSelectedPlanet].position);
                LOG_CRITICAL("Index {}", currentSelectedPlanet)
            }
            if (keyEv.GetKey() == Key::E)
            {
                currentSelectedPlanet--;
                if (currentSelectedPlanet >= solarSystem.size() - 1)
                {
                    currentSelectedPlanet = solarSystem.size() - 1;
                }

                m_Camera->SetCameraTarget(solarSystemSettings[currentSelectedPlanet].position);
                LOG_CRITICAL("Index {}", currentSelectedPlanet)
            }
        }
    }

    if (event.GetType() == EventType::Drop)
    {
        DropEvent dropEv = static_cast<const DropEvent&>(event);
        m_DropPopup.SetData(dropEv.GetFiles());
        m_DropPopup.OpenNewPopup();

        for (auto path : dropEv.GetFiles())
        {
            LOG_CRITICAL("Drop {}", path)
        }
    }
}

void Editor::OnImGuiRender()
{
    BeginGUI();
    //**********************************************************************************************
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open Fille"))
            {
            };
            if (ImGui::MenuItem("Exit"))
            {
            };
            ImGui::EndMenu();
        }


        if (ImGui::BeginMenu("Settings"))
        {
            if (ImGui::MenuItem("Open Settings"))
            {
            };
            ImGui::EndMenu();
        }


        ImGui::EndMenuBar();
    }


    //**********************************************************************************************
    ImGui::Begin("Stats");
    ImGui::Text("FPS %f", m_FrameRate);

    ImGui::Text("Slected \n %s", solarSystemSettings[currentSelectedPlanet].name.c_str());


    ImGui::End();
    //**********************************************************************************************


    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0, 0});
    ImGui::Begin("Viewport");

    // Calculate viewport boundaries and positions
    auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
    auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
    auto viewportOffset = ImGui::GetWindowPos();
    m_ViewportBounds[0] = {viewportMinRegion.x + viewportOffset.x,
                           viewportMinRegion.y + viewportOffset.y};
    m_ViewportBounds[1] = {viewportMaxRegion.x + viewportOffset.x,
                           viewportMaxRegion.y + viewportOffset.y};

    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};

    // Draw the framebuffer image
    ImGui::Image(reinterpret_cast<void*>(m_Framebuffer->GetTextureID()),
                 ImVec2 {m_ViewportSize.x, m_ViewportSize.y},
                 ImVec2 {0, 1},
                 ImVec2 {1, 0});

    ImGui::SetCursorPos(ImVec2(40, 20));

    ImGuiIO& io = ImGui::GetIO();
    ImFont* font = io.Fonts->Fonts[io.Fonts->Fonts.Size - 1];
    ImGui::PushFont(font);

    ImGui::Text(" %s ", solarSystemSettings[currentSelectedPlanet].name.c_str());

    ImGui::PopFont();

    // Setup ImGuizmo for the viewport
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(m_ViewportBounds[0].x,
                      m_ViewportBounds[0].y,
                      m_ViewportBounds[1].x - m_ViewportBounds[0].x,
                      m_ViewportBounds[1].y - m_ViewportBounds[0].y);

    ImGui::End();
    ImGui::PopStyleVar();


    if (ImGui::Begin("Timeline"))
    {
        static int selectedEntry = -1;
        static int firstFrame = 0;

        // Timeline rendering
        Sequencer(&timelineManager,
                  nullptr,
                  nullptr,
                  &selectedEntry,
                  &firstFrame,
                  ImSequencer::SEQUENCER_EDIT_ALL);

        // Async load for selected timeline entry
        if (ImGui::IsItemActive() && selectedEntry >= 0 &&
            selectedEntry < timelineManager.GetTimelineCount())
        {
            const auto& timeline = timelineManager.GetTimelines()[selectedEntry];

            // Lambda function for loading data asynchronously
            auto asyncLoadData = [this, &timeline]() {
                for (int i = 0; i < m_PlotData.size(); i++)
                {
                    if (m_PlotData[i].name == timeline.name && !m_PlotData[i].loaded)
                    {
                        m_PlotData[i].loaded = true;
                        std::ifstream file(timeline.name);

                        if (!file.is_open())
                        {
                            std::cerr
                                << "Error: Could not open file " << timeline.name << std::endl;
                            return;
                        }

                        std::string line;
                        std::lock_guard<std::mutex> lock(
                            plotDataMutex);  // Lock before accessing shared data
                        std::vector<double> timestamps;
                        std::vector<double> values;

                        while (std::getline(file, line))
                        {
                            std::istringstream ss(line);
                            std::string timestampStr, valueStr;

                            // Assuming the CSV has two columns separated by a comma
                            if (std::getline(ss, timestampStr, ',') &&
                                std::getline(ss, valueStr, ','))
                            {
                                try
                                {
                                    // Convert strings to double for plotting
                                    double timestamp = std::stod(timestampStr);
                                    double value = std::stod(valueStr);

                                    timestamps.push_back(timestamp);
                                    values.push_back(value);
                                }
                                catch (const std::exception& e)
                                {
                                    std::cerr << "Error parsing line: " << line << " - " << e.what()
                                              << std::endl;
                                }
                            }
                        }

                        // Store loaded data into m_PlotData
                        m_PlotData[i].plotEntry.timestamps = std::move(timestamps);
                        m_PlotData[i].plotEntry.values = std::move(values);

                        file.close();
                    }
                }
            };

            // Use std::async to load the data asynchronously
            std::future<void> loadFuture = std::async(std::launch::async, asyncLoadData);
        }
    }
    ImGui::End();

    // Plot Visualization for the selected timeline
    ImGui::Begin("Plots");
    if (ImPlot::BeginPlot("Selected Timeline Plot"))
    {
        for (const auto& plotData : m_PlotData)
        {
            if (plotData.loaded)
            {
                std::lock_guard<std::mutex> lock(plotDataMutex);  // Ensure data is safely accessed
                ImPlot::PlotLine(plotData.name.c_str(),
                                 plotData.plotEntry.timestamps.data(),
                                 plotData.plotEntry.values.data(),
                                 plotData.plotEntry.timestamps.size());
            }
        }
        ImPlot::EndPlot();
    }
    ImGui::End();


    m_DropPopup.DrawPopup();


    EndGUI();
}


void Editor::BeginGUI()
{
    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;


    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking |
                                    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);
    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
}
void Editor::EndGUI()
{
    // End of all ImGui
    ImGui::End();
}


}  // namespace Forge

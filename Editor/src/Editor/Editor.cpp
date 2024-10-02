//
// Created by toor on 2024-09-15.
//

#include "Editor.h"


namespace Forge {
Editor::Editor() : Component(ComponentType::None) {}

Editor::~Editor() {}

void Editor::OnAttach()
{
    m_Framebuffer = std::make_shared<Framebuffer>(m_FramebufferProps);
    m_Renderer = std::make_shared<Renderer>();
    m_Renderer->SetClearColor(glm::vec3(0.3, 0.35, 0.4));

    m_Camera = std::make_shared<OrbitalCamera>(glm::vec3(0.0f, 0.0f, 0.0f), 1920, 1080);
    Forge::GetForgeInstance().PushComponent(m_Camera);
}

void Editor::OnDetach() {}
void Editor::OnUpdate(DeltaTime dt)
{
    // Hovered Viewport
    m_Camera->AllowRotation(m_ViewportHovered);
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


    ImGui::End();
    //**********************************************************************************************

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0, 0});
    ImGui::Begin("Viewport");
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
    ImGui::Image(reinterpret_cast<void*>(m_Framebuffer->GetTextureID()),
                 ImVec2 {m_ViewportSize.x, m_ViewportSize.y},
                 ImVec2 {0, 1},
                 ImVec2 {1, 0});


    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();

    ImGuizmo::SetRect(m_ViewportBounds[0].x,
                      m_ViewportBounds[0].y,
                      m_ViewportBounds[1].x - m_ViewportBounds[0].x,
                      m_ViewportBounds[1].y - m_ViewportBounds[0].y);


    ImGui::End();
    ImGui::PopStyleVar();


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

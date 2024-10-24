//
// Created by toor on 2024-09-15.
//

#include "Editor.h"
#include "Forge/Renderer/Renderer2D.h"
#include "glm/fwd.hpp"
#include <iterator>
#include <memory>


namespace Forge {


struct QuadVertex
{
    glm::vec3 Position;
    glm::vec2 TexCoord;
};


Editor::Editor() {}

Editor::~Editor() {}

void Editor::OnAttach()
{
    RenderCommand::SetClearColor(glm::vec3(0.1));

    m_Camera = std::make_shared<EditorCamera>(glm::vec3(0.0f, 0.0f, 0.0f), 1920, 1080);
    m_Camera->SetCameraTarget(glm::vec3(0.0f, 0.0f, 0.0f));  // Look at origin
    Forge::GetForgeInstance().PushModule(m_Camera);

    m_Grid = std::make_shared<Grid>();


    // NOTE: Quad Test
    std::initializer_list<ShaderElement> shaderElements = {
        {ShaderType::VERTEX, "Assets/Shaders/Renderer2DQuad.vert"},
        {ShaderType::FRAGMENT, "Assets/Shaders/Renderer2DQuad.frag"}};
    m_QuadShader = std::make_shared<Shader>(shaderElements);
    m_QuadShader->Build();
    std::vector<QuadVertex> m_Vertices = {
        {{0.25f, 0.25f, 0.0f}, {1.0f, 1.0f}},  // Top Right
        {{-0.25f, -0.25f, 0.0f}, {0.0f, 0.0f}},  // Bottom Left
        {{-0.25f, 0.25f, 0.0f}, {0.0f, 1.0f}},  // Top Left
        {{0.25f, -0.25f, 0.0f}, {1.0f, 0.0f}},  // Bottom Right
    };
    std::vector<unsigned int> m_Indices = {0, 1, 2, 1, 3, 0};
    m_QuadVAO = std::make_shared<VertexArrayBuffer>();
    m_QuadVBO =
        std::make_shared<VertexBuffer>(m_Vertices.data(), m_Vertices.size() * sizeof(QuadVertex));
    m_QuadEBO = std::make_shared<IndexBuffer>(m_Indices.data(), m_Indices.size());
    m_QuadVAO->Bind();
    m_QuadVBO->Bind();
    BufferLayout layout = {{BufferDataType::Float3, "aPos"},
                           {BufferDataType::Float2, "aTexCoords"}};
    m_QuadVBO->SetLayout(layout);
    m_QuadVAO->AddVertexBuffer(m_QuadVBO);
    m_QuadEBO->Bind();
    m_QuadVAO->SetIndexBuffer(m_QuadEBO);
    m_QuadVAO->Unbind();
    // NOTE: Quad Test End
    m_Font = std::make_shared<Font>("/home/toor/fonts//Oswald.ttf");
    if (!m_Font || !m_Font->GetAtlasTexture())
    {
        LOG_ERROR("Failed to load font or its atlas texture");
        return;
    }
}

void Editor::OnDetach() {}


void Editor::OnUpdate(DeltaTime dt)
{
    RenderCommand::Clear();

    std::shared_ptr<Camera> camera = std::static_pointer_cast<Camera>(m_Camera);
    Renderer3D::BeginScene(camera);

    glm::mat4 transform = glm::mat4(1.0f);
    Renderer3D::DrawMesh(m_Grid->GetMesh(m_Camera), transform);

    Renderer3D::EndScene();

    m_QuadShader->Bind();
    m_QuadShader->SetUniform("u_ViewProjection", m_Camera->GetViewProjectionMatrix());

    // Set text color (e.g., white)
    m_QuadShader->SetUniform("u_TextColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    // Set pixel range (should match the range used during MSDF atlas generation)
    float pxRange = 4.0f;  // Example value, adjust as needed
    m_QuadShader->SetUniform("u_PxRange", pxRange);

    // Bind the font's atlas texture
    if (m_Font && m_Font->GetAtlasTexture())
    {
        m_Font->GetAtlasTexture()->Bind(0);
        m_QuadShader->SetUniform("u_Texture", 0);
    }
    else
    {
        LOG_ERROR("Font or atlas texture is not valid");
    }

    m_QuadVAO->Bind();
    RenderCommand::Draw(m_QuadVAO);
}

void Editor::OnEvent(const Event& event)
{
    if (event.GetType() == EventType::Key)
    {
        KeyEvent keyEv = static_cast<const KeyEvent&>(event);
        if (keyEv.GetAction() == Action::KeyPress)
        {
        }
    }

    if (event.GetType() == EventType::Drop)
    {
        DropEvent dropEv = static_cast<const DropEvent&>(event);

        for (auto path : dropEv.GetFiles())
        {
            LOG_CRITICAL("Drop {}", path)
        }
    }


    if (event.GetType() == EventType::Window)
    {
        WindowEvent windowEvent = static_cast<const WindowEvent&>(event);
        if (windowEvent.GetAction() == Action::Resize)
        {
            RenderCommand::SetViewport(0, 0, windowEvent.GetX(), windowEvent.GetY());
            m_Camera->Resize(windowEvent.GetX(), windowEvent.GetY());
        }
    }
}


}  // namespace Forge

//
// Created by toor on 2024-11-21.
//

#include "Button.h"
#include <cstring>
#include <memory>


#include "Forge/BFUI/UIPrimitives/Panel.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Renderer/ShaderManager.h"
#include "Forge/Renderer/Material.h"
#include "glm/fwd.hpp"


namespace BfUI {

Button::Button(glm::vec2 position, glm::vec2 size, const char* label) : m_ButtonBody(position, size)
{
    auto& shaderManager = Forge::ShaderManager::GetInstance();
    // TODO: Shaders accept string as source , but not read from file
    Handle shaderHandle = shaderManager.LoadShader(Forge::ShaderLayout {
        {Forge::ShaderType::VERTEX, "Assets/Shaders/UI/Panel.vert"},
        {Forge::ShaderType::FRAGMENT, "Assets/Shaders/UI/Panel.frag"}});

    if (shaderHandle.GetValue() == 0)
    {
        LOG_CRITICAL("Failed to load shader.");
    }

    m_ButtonMaterial = std::make_shared<Forge::Material>(shaderHandle);
    m_ButtonMaterial->Color = glm::vec4(0.0, 1.0, 0.0, 1.0);


    m_ButtonBufferLayout = {{Forge::BufferDataType::Float2, "a_Position"}};


    UpdateDrawData();
}


Button::~Button() {}


void Button::UpdateDrawData()
{
    std::vector<PanelVertex> vertices = m_ButtonBody.GetVertices();


    m_VerticesBytes.resize(vertices.size() * sizeof(PanelVertex));
    memcpy(m_VerticesBytes.data(), vertices.data(), m_VerticesBytes.size());

    const auto& indices = m_ButtonBody.GetIndices();
    for (auto& index : indices)
    {
        m_Indices.push_back(index);
    }
}

const std::vector<uint8_t>& Button::GetVertices() const
{
    return m_VerticesBytes;
}
const std::vector<uint32_t>& Button::GetIndices() const
{
    return m_Indices;
}

const std::shared_ptr<Forge::Material>& Button::GetMaterial() const
{
    return m_ButtonMaterial;
}


const Forge::BufferLayout& Button::GetLayout() const
{
    return m_ButtonBufferLayout;
}

void Button::SubscribeEvents(std::function<void(BfUI::BfUIEvents events, Button& button)> callback)
{
}


Button& Button::SetBackGroundColor(glm::vec4 color)
{
    m_ButtonMaterial->Color = color;
    return *this;
}


bool Button::InInBounds(glm::vec2 point)
{
    return m_ButtonBody.ContainsPoint(point);
}

}  // namespace BfUI

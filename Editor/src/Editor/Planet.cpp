//
// Created by toor on 2024-10-04.
//

#include "Planet.h"
#include "Forge/Core/Log/Log.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "imgui.h"
#include <glm/gtc/constants.hpp>  // For glm::pi

namespace Forge {

Planet::Planet(PlanetSettings settings)
{
    m_Shader = std::make_shared<Shader>(std::initializer_list<ShaderElement> {
        {ShaderType::VERTEX, "Assets/Shaders/default.vert"},
        {ShaderType::FRAGMENT, "Assets/Shaders/default.frag"}});
    m_Shader->Build();


    vertices.clear();
    indices.clear();

    // Generate sphere mesh
    GenerateSphere(50, 50, vertices, indices);
    m_Mesh = std::make_shared<Mesh>(vertices, indices);


    m_Shader->Bind();
    glm::vec3 lightDirection = glm::vec3(1.0f);
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    m_Shader->SetUniform("u_Light.direction", lightDirection);
    m_Shader->SetUniform("u_Light.color", lightColor);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, settings.position);
    model = glm::scale(model, glm::vec3(settings.scale));
    m_Shader->SetUniform("u_Model", model);


    m_Texture = std::make_shared<Texture>(settings.texture);
}

Planet::~Planet() {}


void Planet::Draw(std::shared_ptr<OrbitalCamera> m_Camera)
{
    m_Shader->Bind();
    glm::vec3 viewPosition = m_Camera->GetCameraPosition();
    m_Shader->SetUniform("u_ViewPos", viewPosition);
    glm::mat4 projectionMatrix = m_Camera->GetViewProjectionMatrix();
    m_Shader->SetUniform("u_ViewProjection", projectionMatrix);


    m_Shader->Bind();

    m_Texture->Bind(0);
    m_Shader->SetUniform("colorTexture", 0);

    m_Mesh->Draw(m_Shader);
}


void Planet::Settings() {}

void Planet::GenerateSphere(int latitudeSegments,
                            int longitudeSegments,
                            std::vector<Vertex>& outVertices,
                            std::vector<unsigned int>& outIndices)
{
    outVertices.clear();
    outIndices.clear();

    for (int y = 0; y <= latitudeSegments; ++y)
    {
        for (int x = 0; x <= longitudeSegments; ++x)
        {
            float xSegment = (float)x / (float)longitudeSegments;
            float ySegment = (float)y / (float)latitudeSegments;
            float xPos = std::cos(xSegment * 2.0f * glm::pi<float>()) *
                         std::sin(ySegment * glm::pi<float>());
            float yPos = std::cos(ySegment * glm::pi<float>());
            float zPos = std::sin(xSegment * 2.0f * glm::pi<float>()) *
                         std::sin(ySegment * glm::pi<float>());

            Vertex vertex;
            vertex.Position = glm::vec3(xPos, yPos, zPos);
            vertex.Normal = glm::normalize(vertex.Position);
            vertex.TexCoords = glm::vec2(1 - xSegment, 1.0f - ySegment);

            outVertices.push_back(vertex);
        }
    }

    // Generate indices
    for (int y = 0; y < latitudeSegments; ++y)
    {
        for (int x = 0; x < longitudeSegments; ++x)
        {
            int first = (y * (longitudeSegments + 1)) + x;
            int second = first + longitudeSegments + 1;

            outIndices.push_back(first);
            outIndices.push_back(second);
            outIndices.push_back(first + 1);

            outIndices.push_back(second);
            outIndices.push_back(second + 1);
            outIndices.push_back(first + 1);
        }
    }
}
}  // namespace Forge

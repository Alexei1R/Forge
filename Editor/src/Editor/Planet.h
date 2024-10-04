//
// Created by toor on 2024-10-04.
//

#ifndef PLANET_H
#define PLANET_H

#include "Forge/Renderer/Mesh.h"
#include "glm/fwd.hpp"
#pragma once

#include <memory>
#include <vector>
#include <Forge.h>

namespace Forge {


struct PlanetSettings
{
    std::string name = "Planet";
    std::string texture = "";
    glm::vec3 position = glm::vec3(0.0f);
    float scale = 1.0f;

    PlanetSettings() : name("Planet"), texture(""), position(glm::vec3(0.0f)), scale(1.0f) {}

    PlanetSettings(const std::string& name,
                   const std::string& texture,
                   const glm::vec3& position,
                   float scale) : name(name), texture(texture), position(position), scale(scale)
    {
    }
};


class Planet
{
public:
    Planet(PlanetSettings settings);
    ~Planet();

    void Draw(std::shared_ptr<OrbitalCamera> m_Camera);

    void GenerateSphere(int latitudeSegments,
                        int longitudeSegments,
                        std::vector<Vertex>& outVertices,
                        std::vector<unsigned int>& outIndices);

    void Settings();

private:
    std::shared_ptr<Shader> m_Shader;
    std::shared_ptr<Mesh> m_Mesh;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    std::shared_ptr<Texture> m_Texture;
    glm::mat4 model;
};

}  // namespace Forge
#endif

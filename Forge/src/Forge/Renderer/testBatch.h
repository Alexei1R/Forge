#include "glm/glm.hpp"
#include <cstdint>
class RenderBatch
{
public:
    struct BatchEntry
    {
        glm::mat4 transform;
        glm::vec4 additionalData;  // Color, custom properties
        uint32_t instanceID;
    };

    RenderBatch(Mesh* mesh, Material* material) : m_Mesh(mesh), m_Material(material) {}

    void AddInstance(const glm::mat4& transform, const glm::vec4& additionalData = glm::vec4(1.0f))
    {
        // Check batch capacity before adding
        if (m_Entries.size() >= MaxInstancesPerBatch)
        {
            Flush();  // Render current batch if full
        }

        m_Entries.push_back({transform, additionalData, static_cast<uint32_t>(m_Entries.size())});
    }

    void Flush()
    {
        if (m_Entries.empty())
            return;

        // Bind material and shader
        m_Material->Bind();

        // Prepare instanced rendering
        PrepareInstanceBuffer();

        // Draw instanced mesh
        m_Mesh->DrawInstanced(m_Entries.size());

        // Clear batch
        m_Entries.clear();
    }

private:
    void PrepareInstanceBuffer()
    {
        // Upload instance data to GPU
        glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
        glBufferSubData(GL_ARRAY_BUFFER,
                        0,
                        m_Entries.size() * sizeof(BatchEntry),
                        m_Entries.data());
    }

    static constexpr size_t MaxInstancesPerBatch = 1024;

    Mesh* m_Mesh;
    Material* m_Material;
    std::vector<BatchEntry> m_Entries;

    // Instancing buffer
    GLuint m_InstanceVBO;
};

class BatchRenderer
{
public:
    void Submit(Mesh* mesh, Material* material, const glm::mat4& transform)
    {
        // Find or create batch for this mesh/material
        auto key = std::make_pair(mesh, material);
        if (m_Batches.find(key) == m_Batches.end())
        {
            m_Batches[key] = std::make_unique<RenderBatch>(mesh, material);
        }

        // Add instance to batch
        m_Batches[key]->AddInstance(transform);
    }

    void Flush()
    {
        for (auto& [key, batch] : m_Batches)
        {
            batch->Flush();
        }
        m_Batches.clear();
    }

private:
    std::map<std::pair<Mesh*, Material*>, std::unique_ptr<RenderBatch>> m_Batches;
};


#ifndef RENDERER_H
#define RENDERER_H

#include <cstdint>
#include <memory>
#include <unordered_map>

#include "Forge/BFUI/Widget.h"
#include "Forge/Renderer/Camera/Camera.h"
#include "Forge/Renderer/Mesh.h"
#include "Forge/Renderer/RendererBatch.h"
#include "Forge/Renderer/UniformBuffer.h"

#define BATCH_VERTEX_SIZE 5000000
#define BATCH_INDEX_SIZE 800000

namespace Forge {

class Renderer {
public:
    Renderer();
    virtual ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    void Begin();
    void End();

    void AddMesh(const MeshTarget& target);

    void Draw(const std::shared_ptr<Camera>& camera);

private:
    void Init();
    bool Shutdown();

private:
    bool m_IsInScope = false;
    std::unique_ptr<UniformBuffer> m_UniformBuffer;
    std::unordered_map<uint32_t, RendererBatch> m_RenderBatches;
};

} // namespace Forge
#endif

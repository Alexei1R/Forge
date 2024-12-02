//
// Created by toor on 2024-11-18.
//

#ifndef RENDERER_H
#define RENDERER_H


#include <cstdint>
#include <memory>
#include <unordered_map>

#include "Forge/BFUI/Widget.h"
#include "Forge/Renderer/Camera/Camera.h"
#include "Forge/Renderer/Material.h"
#include "Forge/Renderer/RendererBatch.h"
#include "Forge/Renderer/UniformBuffer.h"
#include "Forge/Renderer/RenderableTarget.h"

namespace Forge {


class Renderer
{
public:
    Renderer() = delete;
    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;
    ~Renderer() = delete;

    static bool Initialize();
    static bool Shutdown();
    static void Begin(const std::shared_ptr<Camera>& camera);
    static void End();
    static void SubmitMesh(const MeshTarget& target);

    static void SubmitMesh(const RenderableTarget& target,
                           const std::shared_ptr<Material>& material);

    static void SubmitText(const RenderableTarget& target,
                           const std::shared_ptr<Material>& material);

    static void SubmitUIElement(const BfUI::Widget& widget);
    static void SetPrecalculatedUIElementSize(uint32_t vertices, uint32_t indices);

private:
    static void CheckScope();

private:
    static bool m_IsInScope;
    static bool m_IsInitialized;
    static std::unique_ptr<UniformBuffer> m_UniformBuffer;
    static std::unordered_map<uint32_t, RendererBatch> m_RenderBatches;
    static uint32_t m_PrecalculatedVerticesSize;
    static uint32_t m_PrecalculatedIndicesSize;
};


}  // namespace Forge
#endif

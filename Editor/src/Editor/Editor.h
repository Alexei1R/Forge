//
// Created by toor on 2024-09-15.
//

#ifndef EDITOR_H
#define EDITOR_H

#include "Forge.h"
#include "Forge/BFUI/Button.h"
#include "Forge/Renderer/Material.h"
#include "Forge/Renderer/RenderableTargets/Quad.h"
#include "Forge/Renderer/RenderableTargets/Text.h"
#include <memory>


namespace Forge {


class Editor : public Module
{
public:
    Editor();
    ~Editor();

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(DeltaTime dt) override;
    void OnEvent(const Event& event) override;


private:
    std::shared_ptr<Camera> m_Camera;
    std::shared_ptr<Camera> m_CameraScreenSpace;


    float timeAccumulator = 0.0f;
    int frameCount = 0;
    float fpsAverage = 0.0f;
    std::shared_ptr<MaterialManager> materialManager;
    std::shared_ptr<Quad> m_Quad;

    std::shared_ptr<BfUI::Button> button;

    std::shared_ptr<Text> m_Text;
    float m_Width = 0;
    float m_Height = 0;
};

}  // namespace Forge


#endif

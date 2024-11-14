//
// Created by toor on 2024-09-15.
//

#ifndef EDITOR_H
#define EDITOR_H

#include "Forge.h"
#include "Forge/Renderer/Material.h"
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
    std::shared_ptr<EditorCamera> m_Camera;


    std::shared_ptr<MaterialManager> materialManager;
    Renderer2D renderer;


    float timeAccumulator = 0.0f;
    int frameCount = 0;
    float fpsAverage = 0.0f;


    float m_Width = 0;
    float m_Height = 0;
};

}  // namespace Forge


#endif

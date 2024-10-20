//
// Created by toor on 2024-09-15.
//

#ifndef EDITOR_H
#define EDITOR_H

#include "Forge.h"
#include "src/Editor/Grid.h"
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
    std::shared_ptr<Renderer> m_Renderer;
    std::shared_ptr<EditorCamera> m_Camera;


    bool m_ShowDebugGrid = true;
    std::shared_ptr<Grid> m_Grid;
};

}  // namespace Forge


#endif

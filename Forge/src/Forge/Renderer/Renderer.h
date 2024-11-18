//
// Created by toor on 2024-11-18.
//

#ifndef RENDERER_H
#define RENDERER_H

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

    bool Initialize();
    bool Shutdown();


    void Begin();
    void End();
    void Submit();

private:
    void CheckScope();

private:
    static bool m_IsInScope;
    static bool m_IsInitialized;
};


}  // namespace Forge
#endif

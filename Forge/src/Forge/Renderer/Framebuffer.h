
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H


#include <vector>

namespace Forge {


enum class FrameTextureFormat
{
    None = 0,
    // Color
    RGBA8,
    RED_INTEGER,
    // Depth/stencil
    DEPTH24STENCIL8,
    // Defaults
    Depth = DEPTH24STENCIL8
};


struct FramebufferProps
{
    int height;
    int width;
    FrameTextureFormat format;
    FramebufferProps(int width = 1920,
                     int height = 1080,
                     FrameTextureFormat format = FrameTextureFormat::RGBA8) :
        width(width), height(height), format(format) {};
};


class Framebuffer
{
public:
    Framebuffer(const FramebufferProps& spec);
    ~Framebuffer();


    void Bind();
    void UnBind();

    void Resize(unsigned int width, unsigned int height);

    unsigned int GetTextureID() const;

    const FramebufferProps& GetSpecification() const
    {
        return m_Specification;
    }

private:
    unsigned int m_RendererID;
    unsigned int m_TextureID;
    unsigned int m_Rbo;

    FramebufferProps m_Specification;
};

}  // namespace Forge


#endif

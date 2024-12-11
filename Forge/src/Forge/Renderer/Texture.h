

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <glad/glad.h>

namespace Forge {

class Texture
{
public:
    Texture(const std::string& filePath);
    Texture(int width, int height, GLenum format, const void* data = nullptr);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void UnBind() const;


    unsigned int GetID() const
    {
        return m_TextureID;
    }

    int GetWidth() const;
    int GetHeight() const;
    GLenum GetFormat() const;

private:
    void InitializeTexture(const void* data);

private:
    unsigned int m_TextureID;
    std::string m_FilePath;
    int m_Width, m_Height;
    GLenum m_Format;
};

}  // namespace Forge

#endif  // TEXTURE_H

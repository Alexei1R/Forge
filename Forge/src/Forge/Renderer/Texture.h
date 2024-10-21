//
// Created by toor on 2024-09-13.
//


#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <glad/glad.h>

namespace Forge {


enum class TextureType
{
    Albedo,  // Base color or diffuse texture
    Normal,  // Normal map for surface detail
    Metallic,  // Metallic map for PBR workflow
    Roughness,  // Roughness map, controls surface smoothness
    Height,  // Height or displacement map for surface depth
    AmbientOcclusion,  // Ambient occlusion map for shadowing detail
    Specular,  // Specular map for shiny or reflective surfaces
    Emission,  // Emission map for glowing surfaces
    Opacity,  // Transparency or alpha map for opacity control
    Detail,  // Detail normal or additional surface details
    Displacement  // Displacement map for actual geometry displacement
};

class Texture
{
public:
    Texture(const std::string& filePath, TextureType type = TextureType::Albedo);

    Texture(int width, int height, GLenum format, const void* data = nullptr);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void UnBind() const;

    std::string& GetTexturePath()
    {
        return m_FilePath;
    }

    unsigned int GetID() const
    {
        return m_TextureID;
    }

    TextureType GetTextureType()
    {
        return m_TextureType;
    }

    int GetWidth() const;
    int GetHeight() const;
    GLenum GetFormat() const;

private:
    void InitializeTexture(const void* data);

private:
    TextureType m_TextureType;


    unsigned int m_TextureID;
    std::string m_FilePath;
    int m_Width, m_Height;
    GLenum m_Format;
};

}  // namespace Forge

#endif  // TEXTURE_H

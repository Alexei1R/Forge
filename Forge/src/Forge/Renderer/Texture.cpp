
#include "Texture.h"

#include <stb/stb_image.h>

#include "Forge/Core/Log/Log.h"

namespace Forge {

Texture::Texture(const std::string& filePath)
    : m_FilePath(filePath)
    , m_Width(0)
    , m_Height(0)
    , m_Format(GL_RGB)
    , m_TextureID(0) {
    stbi_set_flip_vertically_on_load(true);
    int nrChannels;
    unsigned char* data = stbi_load(filePath.c_str(), &m_Width, &m_Height, &nrChannels, 0);

    if (data) {
        if (nrChannels == 1)
            m_Format = GL_RED;
        else if (nrChannels == 3)
            m_Format = GL_RGB;
        else if (nrChannels == 4)
            m_Format = GL_RGBA;
        else
            m_Format = GL_RGB;
        InitializeTexture(data);

        stbi_image_free(data);
    } else {
        LOG_CRITICAL("Failed to load texture: {}", filePath)
        stbi_image_free(data);
    }
}

Texture::Texture(int width, int height, GLenum format, const void* data)
    : m_Width(width)
    , m_Height(height)
    , m_Format(format)
    , m_TextureID(0) {
    InitializeTexture(data);
}

Texture::~Texture() {
    glDeleteTextures(1, &m_TextureID);
}

void Texture::Bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Texture::UnBind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture::GetWidth() const {
    return m_Width;
}

int Texture::GetHeight() const {
    return m_Height;
}

GLenum Texture::GetFormat() const {
    return m_Format;
}

void Texture::InitializeTexture(const void* data) {
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (data) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, m_Format, m_Width, m_Height, 0, m_Format, GL_UNSIGNED_BYTE, data);

    if (data)
        glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace Forge

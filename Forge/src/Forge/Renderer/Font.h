

#ifndef FONT_H
#define FONT_H

#include <string>
#include <unordered_map>
#include <memory>
#include <filesystem>
#include "Forge/Core/Handle.h"
#include "Forge/Renderer/TextureManager.h"
#include <glm/glm.hpp>
#include "msdfgen.h"
#include "msdf-atlas-gen/msdf-atlas-gen.h"

namespace Forge {


struct MSDFData
{
    std::vector<msdf_atlas::GlyphGeometry> Glyphs;
    msdf_atlas::FontGeometry FontGeometry;
};


class Font
{
public:
    Font(const std::filesystem::path& font);
    ~Font();

    const MSDFData* GetMSDFData() const
    {
        return m_Data;
    }
    Handle GetAtlasTextureHandle() const

    {
        return m_AtlasTextureHandle;
    }

    static std::shared_ptr<Font> GetDefault();

private:
    MSDFData* m_Data;
    Handle m_AtlasTextureHandle;
};

}  // namespace Forge

#endif  // FONT_H

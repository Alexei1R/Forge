//
// Created by toor on 2024-10-21.
//

#ifndef TEXT_H
#define TEXT_H

#include "Forge/Renderer/Texture.h"
#include "msdf-atlas-gen/msdf-atlas-gen.h"
#include "msdfgen.h"
#include <algorithm>
#include <filesystem>
#include <memory>
#include <string>


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
    std::shared_ptr<Texture> GetAtlasTexture() const
    {
        return m_AtlasTexture;
    }

    static std::shared_ptr<Font> GetDefault();

private:
    MSDFData* m_Data;
    std::shared_ptr<Texture> m_AtlasTexture;
};


}  // namespace Forge

#endif



#ifndef FONT_H
#define FONT_H

#include <filesystem>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>

#include "Forge/Core/Handle.h"
#include "Forge/Renderer/TextureManager.h"
#include "msdf-atlas-gen/msdf-atlas-gen.h"
#include "msdfgen.h"

namespace Forge {

struct MSDFData {
    std::vector<msdf_atlas::GlyphGeometry> Glyphs;
    msdf_atlas::FontGeometry FontGeometry;
};

class Font {
public:
    Font(const std::filesystem::path& font);
    ~Font();

    const MSDFData* GetMSDFData() const {
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

} // namespace Forge

#endif // FONT_H

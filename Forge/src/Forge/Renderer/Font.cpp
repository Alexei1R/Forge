

#include "Font.h"

#include "Forge/Core/Log/Log.h"
#include "Forge/Core/Utils.h"

namespace Forge {

std::optional<std::string> find_default_font(const std::vector<std::string>& directories) {
    for (const auto& dir : directories) {
        if (std::filesystem::exists(dir) && std::filesystem::is_directory(dir)) {
            for (const auto& entry : std::filesystem::directory_iterator(dir)) {
                if (entry.is_regular_file()) {
                    std::string extension = entry.path().extension().string();
                    if (extension == ".ttf" || extension == ".otf" || extension == ".ttc") {
                        return entry.path().string();
                    }
                    LOG_INFO("{}", entry.path().string())
                }
            }
        }
    }
    return std::nullopt; // No font found
}

template <typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
static Handle CreateAndCacheAtlas(const std::string& fontName, float fontSize, const std::vector<msdf_atlas::GlyphGeometry>& glyphs,
                                  const msdf_atlas::FontGeometry& fontGeometry, uint32_t width, uint32_t height) {
    msdf_atlas::GeneratorAttributes attributes;
    attributes.config.overlapSupport = true;
    attributes.scanlinePass = true;

    msdf_atlas::ImmediateAtlasGenerator<S, N, GenFunc, msdf_atlas::BitmapAtlasStorage<T, N>> generator(width, height);
    generator.setAttributes(attributes);
    generator.setThreadCount(8);
    generator.generate(glyphs.data(), (int)glyphs.size());

    msdfgen::BitmapConstRef<T, N> bitmap = (msdfgen::BitmapConstRef<T, N>)generator.atlasStorage();

    // Create texture and store it in TextureManager
    Handle textureHandle = TextureManager::GetInstance().DataTexture(bitmap.width, bitmap.height, GL_RGB, bitmap.pixels);

    return textureHandle;
}

Font::Font(const std::filesystem::path& filepath)
    : m_Data(new MSDFData()) {
    msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();

    std::string fileString = filepath.string();

    msdfgen::FontHandle* font = msdfgen::loadFont(ft, fileString.c_str());
    if (!font) {
        LOG_CRITICAL("Failed to load font: {}", fileString);
        return;
    }

    struct CharsetRange {
        uint32_t Begin, End;
    };

    // From imgui_draw.cpp
    static const CharsetRange charsetRanges[] = {{0x0020, 0x00FF}};

    msdf_atlas::Charset charset;
    for (CharsetRange range : charsetRanges) {
        for (uint32_t c = range.Begin; c <= range.End; c++)
            charset.add(c);
    }

    double fontScale = 1.0;
    m_Data->FontGeometry = msdf_atlas::FontGeometry(&m_Data->Glyphs);
    int glyphsLoaded = m_Data->FontGeometry.loadCharset(font, fontScale, charset);
    LOG_INFO("Loaded {} glyphs from font (out of {})", glyphsLoaded, charset.size());

    double emSize = 40.0;

    msdf_atlas::TightAtlasPacker atlasPacker;
    // atlasPacker.setDimensionsConstraint()
    atlasPacker.setPixelRange(2.0);
    atlasPacker.setMiterLimit(1.0);
    atlasPacker.setScale(emSize);
    int remaining = atlasPacker.pack(m_Data->Glyphs.data(), (int)m_Data->Glyphs.size());

    int width, height;
    atlasPacker.getDimensions(width, height);
    emSize = atlasPacker.getScale();

#define DEFAULT_ANGLE_THRESHOLD 3.0
#define LCG_MULTIPLIER 6364136223846793005ull
#define LCG_INCREMENT 1442695040888963407ull
#define THREAD_COUNT 8
    // if MSDF || MTSDF

    uint64_t coloringSeed = 0;
    bool expensiveColoring = false;
    if (expensiveColoring) {
        msdf_atlas::Workload(
            [&glyphs = m_Data->Glyphs, &coloringSeed](int i, int threadNo) -> bool {
                unsigned long long glyphSeed = (LCG_MULTIPLIER * (coloringSeed ^ i) + LCG_INCREMENT) * !!coloringSeed;
                glyphs[i].edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
                return true;
            },
            m_Data->Glyphs.size())
            .finish(THREAD_COUNT);
    } else {
        unsigned long long glyphSeed = coloringSeed;
        for (msdf_atlas::GlyphGeometry& glyph : m_Data->Glyphs) {
            glyphSeed *= LCG_MULTIPLIER;
            glyph.edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
        }
    }

    LOG_INFO("Font W {} H {} ", width, height)
    m_AtlasTextureHandle = CreateAndCacheAtlas<uint8_t, float, 3, msdf_atlas::msdfGenerator>("Test", (float)emSize, m_Data->Glyphs,
                                                                                             m_Data->FontGeometry, width, height);

    msdfgen::destroyFont(font);
    msdfgen::deinitializeFreetype(ft);
}

Font::~Font() {
    delete m_Data;
}

std::shared_ptr<Font> Font::GetDefault() {
    static std::shared_ptr<Font> DefaultFont;
    if (!DefaultFont) {
        std::vector<std::string> font_directories;
#ifdef _WIN32
        font_directories.push_back("C:\\Windows\\Fonts");
#else
        // Expand '~' to the user's home directory
        const char* home_dir = std::getenv("HOME");
        if (home_dir) {
            font_directories.push_back(std::string(home_dir) + "/.fonts");
            font_directories.push_back(std::string(home_dir) + "/.local/share/fonts");
        }
        font_directories.push_back("/usr/share/fonts");
#endif

        auto font_path = find_default_font(font_directories);

        if (font_path) {
            LOG_TRACE("Default font found: ", *font_path)
        } else {
            F_ASSERT(false, "No font found in default directories.\n")
        }

        if (font_path) {
            DefaultFont = std::make_shared<Font>(std::filesystem::path(*font_path));
        } else {
            LOG_CRITICAL("No default font available. Returning null shared pointer.");
            DefaultFont = nullptr;
        }
    }
    return DefaultFont;
}
} // namespace Forge

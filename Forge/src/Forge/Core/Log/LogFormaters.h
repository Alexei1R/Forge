#ifndef LOGFORMATERS_H
#define LOGFORMATERS_H

#include <fmt/format.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>

// Specialize `fmt` for `glm::vec2`
template <>
struct fmt::formatter<glm::vec2> {
    constexpr auto parse(fmt::format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const glm::vec2& vec, FormatContext& ctx) const { // Make format const
        return fmt::format_to(ctx.out(), "({}, {})", vec.x, vec.y);
    }
};

// Specialize `fmt` for `glm::vec3`
template <>
struct fmt::formatter<glm::vec3> {
    constexpr auto parse(fmt::format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const glm::vec3& vec, FormatContext& ctx) const { // Make format const
        return fmt::format_to(ctx.out(), "({}, {}, {})", vec.x, vec.y, vec.z);
    }
};

// Specialize `fmt` for `glm::vec4`
template <>
struct fmt::formatter<glm::vec4> {
    constexpr auto parse(fmt::format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const glm::vec4& vec, FormatContext& ctx) const { // Make format const
        return fmt::format_to(ctx.out(), "({}, {}, {}, {})", vec.x, vec.y, vec.z, vec.w);
    }
};

// Specialize `fmt` for `glm::mat3`
template <>
struct fmt::formatter<glm::mat3> {
    constexpr auto parse(fmt::format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const glm::mat3& mat, FormatContext& ctx) const { // Make format const
        return fmt::format_to(ctx.out(), "\n[{}, {}, {}]\n[{}, {}, {}]\n[{}, {}, {}]", mat[0][0], mat[0][1], mat[0][2], mat[1][0],
                              mat[1][1], mat[1][2], mat[2][0], mat[2][1], mat[2][2]);
    }
};

// Specialize `fmt` for `glm::mat4`
template <>
struct fmt::formatter<glm::mat4> {
    constexpr auto parse(fmt::format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const glm::mat4& mat, FormatContext& ctx) const { // Make format const
        return fmt::format_to(ctx.out(),
                              "\n[{}, {}, {}, {}]\n[{}, {}, {}, {}]\n[{}, {}, {}, "
                              "{}]\n[{}, {}, {}, {}]",
                              mat[0][0], mat[0][1], mat[0][2], mat[0][3], mat[1][0], mat[1][1], mat[1][2], mat[1][3], mat[2][0],
                              mat[2][1], mat[2][2], mat[2][3], mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
    }
};

#endif // GLM_FORMATTERS_H

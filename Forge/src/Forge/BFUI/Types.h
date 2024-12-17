
#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <glm/glm.hpp>

namespace bf {

// 2D vector types
using vec2i = glm::vec<2, int32_t>;
using vec2f = glm::vec<2, float>;
using vec2u = glm::vec<2, uint32_t>;

// 3D vector types
using vec3i = glm::vec<3, int32_t>;
using vec3f = glm::vec<3, float>;
using vec3u = glm::vec<3, uint32_t>;

// 4D vector types
using vec4i = glm::vec<4, int32_t>;
using vec4f = glm::vec<4, float>;
using vec4u = glm::vec<4, uint32_t>;

} // namespace bf

#endif

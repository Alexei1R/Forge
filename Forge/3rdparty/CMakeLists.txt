add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(glm)
add_subdirectory(spdlog)
add_subdirectory(stb)


set(MSDF_ATLAS_BUILD_STANDALONE OFF CACHE BOOL "Do not build standalone executable" FORCE)
set(MSDF_ATLAS_USE_VCPKG OFF CACHE BOOL "Do not use vcpkg" FORCE)
set(MSDF_ATLAS_INSTALL OFF CACHE BOOL "Disable installation" FORCE)
set(MSDF_ATLAS_MSDFGEN_EXTERNAL OFF CACHE BOOL "Disable external MSDFGEN" FORCE)
set(MSDF_ATLAS_NO_ARTERY_FONT ON CACHE BOOL "Do not use artery font" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static library" FORCE)
set(MSDF_ATLAS_USE_SKIA OFF CACHE BOOL "Do not use Skia" FORCE)

add_subdirectory(msdf-atlas-gen)


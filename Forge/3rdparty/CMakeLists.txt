add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(glm)
add_subdirectory(spdlog)
add_subdirectory(stb)


# NOTE: Disable building code for image generation 
set(MSDF_ATLAS_BUILD_STANDALONE OFF CACHE BOOL "Do not build standalone executable" FORCE)
set(MSDF_ATLAS_USE_VCPKG OFF CACHE BOOL "Do not use vcpkg" FORCE)
set(MSDF_ATLAS_INSTALL OFF CACHE BOOL "Disable installation" FORCE)
set(MSDF_ATLAS_MSDFGEN_EXTERNAL OFF CACHE BOOL "Disable external MSDFGEN" FORCE)
set(MSDF_ATLAS_NO_ARTERY_FONT ON CACHE BOOL "Do not use artery font" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static library" FORCE)
set(MSDF_ATLAS_USE_SKIA OFF CACHE BOOL "Do not use Skia" FORCE)
add_subdirectory(msdf-atlas-gen)



# NOTE: Basic options to minimize build
option(ASSIMP_BUILD_ASSIMP_TOOLS "Build Assimp tools" OFF)
option(ASSIMP_BUILD_TESTS "Build Assimp tests" OFF)
option(ASSIMP_BUILD_SAMPLES "Build Assimp samples" OFF)
option(ASSIMP_NO_EXPORT "Disable Assimp export functionality" ON)
option(ASSIMP_BUILD_DOCS "Build documentation" OFF)
option(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT "Build all importers by default" OFF)
option(ASSIMP_BUILD_OBJ_IMPORTER "Enable OBJ importer" ON)
option(ASSIMP_BUILD_FBX_IMPORTER "Enable FBX importer" ON)

# Disable unused importers
set(ASSIMP_BUILD_GLTF_IMPORTER OFF CACHE BOOL "Disable GLTF importer")
set(ASSIMP_BUILD_COLLADA_IMPORTER OFF CACHE BOOL "Disable Collada importer")
set(ASSIMP_BUILD_BLEND_IMPORTER OFF CACHE BOOL "Disable Blender importer")

# Precompiled headers for faster build
option(ASSIMP_USE_PRECOMPILED_HEADERS "Enable precompiled headers" ON)

if(ASSIMP_BUILD_OBJ_IMPORTER)
    add_definitions(-DFORGE_IMPORT_AVAILABLE_OBJ)
endif()

if(ASSIMP_BUILD_FBX_IMPORTER)
    add_definitions(-DFORGE_IMPORT_AVAILABLE_FBX)
endif()



add_subdirectory(assimp)


# Output message to confirm configuration
message(STATUS "Assimp Minimal Build Configuration:")
message(STATUS " - OBJ Importer: ${ASSIMP_BUILD_OBJ_IMPORTER}")
message(STATUS " - FBX Importer: ${ASSIMP_BUILD_FBX_IMPORTER}")
message(STATUS " - Precompiled Headers: ${ASSIMP_USE_PRECOMPILED_HEADERS}")

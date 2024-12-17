
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries by default")

file(GLOB_RECURSE LIB_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
file(GLOB_RECURSE LIB_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(ForgeLIB ${LIB_SRC} ${LIB_HEADERS})

if(BUILD_SHARED_LIBS)
    target_compile_definitions(ForgeLIB PRIVATE FORGE_BUILD_SHARED)
    message(STATUS "Building ForgeLIB as a shared library")
else()
    target_compile_definitions(ForgeLIB PRIVATE FORGE_BUILD_STATIC)
    message(STATUS "Building ForgeLIB as a static library")
endif()

target_link_libraries(ForgeLIB
    spdlog
    glm
    glfw
    glad
    stb
    msdf-atlas-gen
    assimp
)

target_include_directories(ForgeLIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)

if(ASSIMP_BUILD_OBJ_IMPORTER)
    target_compile_definitions(ForgeLIB PRIVATE FORGE_IMPORT_AVAILABLE_OBJ)
endif()

if(ASSIMP_BUILD_FBX_IMPORTER)
    target_compile_definitions(ForgeLIB PRIVATE FORGE_IMPORT_AVAILABLE_FBX)
endif()

add_compile_definitions(LIBRARY_DIR="${CMAKE_CURRENT_SOURCE_DIR}/src/Forge")



add_subdirectory(3rdparty)

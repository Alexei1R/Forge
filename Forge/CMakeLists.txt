

add_subdirectory(3rdparty)


file(GLOB_RECURSE LIB_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

file(GLOB_RECURSE LIB_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(ForgeLIB ${LIB_SRC} ${LIB_HEADERS})




target_link_libraries(ForgeLIB
        spdlog
        glm
        glfw
        glad
        stb
        msdf-atlas-gen
        assimp
)

# Instead of add_definitions(), use target_compile_definitions()
if(ASSIMP_BUILD_OBJ_IMPORTER)
    target_compile_definitions(ForgeLIB PRIVATE FORGE_IMPORT_AVAILABLE_OBJ)
endif()

if(ASSIMP_BUILD_FBX_IMPORTER)
    target_compile_definitions(ForgeLIB PRIVATE FORGE_IMPORT_AVAILABLE_FBX)
endif()

target_include_directories(ForgeLIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)



add_compile_definitions(LIBRARY_DIR="${CMAKE_CURRENT_SOURCE_DIR}/src/Forge")



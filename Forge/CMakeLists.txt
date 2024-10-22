

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
)


target_include_directories(ForgeLIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)



add_compile_definitions(LIBRARY_DIR="${CMAKE_CURRENT_SOURCE_DIR}/src/Forge")



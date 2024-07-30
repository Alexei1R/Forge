

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE LIB_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/Forge/*.cpp"
)

file(GLOB_RECURSE LIB_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/Forge/*.h"
)

add_library(Forge ${LIB_SRC} ${LIB_HEADERS})




target_link_libraries(Forge
        glfw
        glm
        spdlog
)

if(BUILD_BACKEND_OPENGL)
    target_link_libraries(Forge glad)
endif()

target_include_directories(Forge PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)

add_subdirectory(backends)


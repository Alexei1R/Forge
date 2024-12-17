


file(GLOB_RECURSE EXECUTABLE_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

file(GLOB_RECURSE EXECUTABLE_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(sandbox ${EXECUTABLE_SRC} ${EXECUTABLE_HEADERS})

target_link_libraries(sandbox
    ForgeLIB
)

target_include_directories(sandbox PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Forge_LIBS}
)


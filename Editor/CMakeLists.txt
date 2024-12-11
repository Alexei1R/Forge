


file(GLOB_RECURSE EXECUTABLE_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

file(GLOB_RECURSE EXECUTABLE_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(ForgeEditor ${EXECUTABLE_SRC} ${EXECUTABLE_HEADERS})

target_link_libraries(ForgeEditor
    ForgeLIB
)

target_include_directories(ForgeEditor PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Forge_LIBS}
)


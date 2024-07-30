# add_subdirectory(Vulkan)


if(BUILD_BACKEND_OPENGL)
    message("Linking the OpenGL backend")
    add_definitions(-DBUILD_BACKEND_OPENGL)
    add_subdirectory(OpenGL)
endif()



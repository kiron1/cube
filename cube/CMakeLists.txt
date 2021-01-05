#=============================================================================
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt
#=============================================================================

add_library(cube_gloo
    gloo.hpp
    gloo/attribute.hpp
    gloo/buffer.hpp
    gloo/detail/variable.hpp
    gloo/element_index.hpp
    gloo/globject.hpp
    gloo/opengl.hpp
    gloo/program.hpp
    gloo/shader.hpp
    gloo/uniform.hpp
    gloo/variable.hpp
    gloo/window.hpp
    gloo/attribute.cpp
    gloo/buffer.cpp
    gloo/element_index.cpp
    gloo/program.cpp
    gloo/shader.cpp
    gloo/uniform.cpp
    gloo/window.cpp
)
target_include_directories(cube_gloo
    PUBLIC
        $<BUILD_INTERFACE:${Cube_SOURCE_DIR}>
)
target_compile_options(cube_gloo
    PUBLIC
        $<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++>
)
target_link_libraries(cube_gloo
    PUBLIC
        Boost::headers
        OpenGL::GL
        GLFW::GLFW
        cube_glad
        $<$<CXX_COMPILER_ID:Clang>:c++>
)
add_library(${PROJECT_NAME}::gloo ALIAS cube_gloo)
set_target_properties(cube_gloo PROPERTIES
    EXPORT_NAME "gloo"
)

install(TARGETS cube_gloo EXPORT ${PROJECT_NAME}
    RUNTIME       DESTINATION ${CMAKE_INSTALL_BINDIR}     COMPONENT "${PROJECT_NAME}"
    LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}     COMPONENT "${PROJECT_NAME}"
    ARCHIVE       DESTINATION ${CMAKE_INSTALL_LIBDIR}     COMPONENT "${PROJECT_NAME}-dev"
    INCLUDES      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT "${PROJECT_NAME}-dev"
)


#=============================================================================
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt
#=============================================================================

add_library(cube_gloo
    ${Cube_SOURCE_DIR}/include/cube/gloo.hpp
    ${Cube_SOURCE_DIR}/include/cube/gloo/attribute.hpp
    ${Cube_SOURCE_DIR}/include/cube/gloo/buffer.hpp
    ${Cube_SOURCE_DIR}/include/cube/gloo/detail/variable.hpp
    ${Cube_SOURCE_DIR}/include/cube/gloo/element_index.hpp
    ${Cube_SOURCE_DIR}/include/cube/gloo/globject.hpp
    ${Cube_SOURCE_DIR}/include/cube/gloo/opengl.hpp
    ${Cube_SOURCE_DIR}/include/cube/gloo/program.hpp
    ${Cube_SOURCE_DIR}/include/cube/gloo/shader.hpp
    ${Cube_SOURCE_DIR}/include/cube/gloo/uniform.hpp
    ${Cube_SOURCE_DIR}/include/cube/gloo/variable.hpp
    ${Cube_SOURCE_DIR}/include/cube/gloo/window.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/attribute.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/buffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/element_index.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/program.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/uniform.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/window.cpp
)
target_include_directories(cube_gloo
    PUBLIC
        $<BUILD_INTERFACE:${Cube_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${Cube_BINARY_DIR}/include>
    PRIVATE
        src
)
target_compile_options(cube_gloo
    PUBLIC
        $<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++>
)
target_link_libraries(cube_gloo
    PUBLIC
        Boost::headers
        OpenGL::GL
        GLEW::GLEW
        GLUT::GLUT
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


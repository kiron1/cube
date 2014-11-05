#=============================================================================
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt
#=============================================================================

add_executable(display_shape display_shape.cpp)
target_link_libraries(display_shape
    PRIVATE
        ${PROJECT_NAME}::gloo
)

add_executable(display_shape_rawgl display_shape_rawgl.cpp)
target_link_libraries(display_shape_rawgl
    PRIVATE
        ${PROJECT_NAME}::gloo
)


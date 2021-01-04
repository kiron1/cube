#=============================================================================
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt
#=============================================================================

add_library(cube_glad
    include/glad/glad.h
    src/glad.c
)
target_include_directories(cube_glad
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        src
)

add_library(${PROJECT_NAME}::glad ALIAS cube_glad)
set_target_properties(cube_glad PROPERTIES
    EXPORT_NAME "glad"
)

install(TARGETS cube_glad EXPORT ${PROJECT_NAME}
    RUNTIME       DESTINATION ${CMAKE_INSTALL_BINDIR}     COMPONENT "${PROJECT_NAME}"
    LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}     COMPONENT "${PROJECT_NAME}"
    ARCHIVE       DESTINATION ${CMAKE_INSTALL_LIBDIR}     COMPONENT "${PROJECT_NAME}-dev"
    INCLUDES      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT "${PROJECT_NAME}-dev"
)


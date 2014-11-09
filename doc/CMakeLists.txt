#=============================================================================
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt
#=============================================================================

if(BUILD_DOCUMENTATION)
    add_boostbook(html ALL
        SOURCES
            cube.qbk
            preface.qbk
            introduction.qbk
            build.qbk
            variables.qbk
            examples.qbk
            examples/display_shape.qbk
            developers.qbk
            references.qbk
            ../include/cube/gloo/buffer.hpp
            ../include/cube/gloo/attribute.hpp
            ../include/cube/gloo/element_index.hpp
            ../include/cube/gloo/variable.hpp
            ../include/cube/gloo/shader.hpp
            ../include/cube/gloo/window.hpp
            ../include/cube/gloo/globject.hpp
            ../include/cube/gloo/opengl.hpp
            ../include/cube/gloo/program.hpp
            ../include/cube/gloo/uniform.hpp
            ../include/cube/gloo.hpp
    )
    add_custom_target(doc)
    add_dependencies(doc html)

    install(DIRECTORY html
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
        COMPONENT "${PROJECT_NAME}-doc"
    )

    if(${PROJECT_NAME}_DEVELOPER AND TARGET Git::git AND EXISTS "${CMAKE_SOURCE_DIR}/.git/config")
        set(_ghpages_dir "${CMAKE_CURRENT_BINARY_DIR}/gh-pages")
        set(_ghpages_output "${_ghpages_dir}/.git/config")
        add_custom_command(
            OUTPUT "${_ghpages_output}"
            DEPENDS "doc"
            COMMAND ${CMAKE_COMMAND} -E remove_directory "${_ghpages_dir}"
            COMMAND Git::git clone -b gh-pages "${CMAKE_SOURCE_DIR}/.git" "${_ghpages_dir}"
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_BINARY_DIR}/html/" "${_ghpages_dir}"
            COMMAND ${CMAKE_COMMAND} -E touch_nocreate "${_ghpages_output}"
            COMMENT "Updating gh-pages..."
        )
        add_custom_target(publish-doc
            DEPENDS "${_ghpages_output}"
            COMMAND Git::git add "${_ghpages_dir}"
            COMMAND Git::git commit -m "updated gh-pages"
            WORKING_DIRECTORY "${_ghpages_dir}"
        )
    endif()
endif()

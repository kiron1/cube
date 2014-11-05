#=============================================================================
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt
#=============================================================================

if(DOXYGEN_EXECUTABLE)
    set(_doxygen_output_dir "${CMAKE_CURRENT_BINARY_DIR}/html")
    set(_doxygen_output_file "${_doxygen_output_dir}/index.html")
    set(_doxygen_example_path "${PROJECT_SOURCE_DIR}/examples")

    file(GLOB_RECURSE _doxygen_input
        "${PROJECT_SOURCE_DIR}/include/*.hpp"
        "${PROJECT_SOURCE_DIR}/doc/*.md"
    )
    file(GLOB_RECURSE _doxygen_examples
        "${_doxygen_example_path}/*.cpp"
    )
    list(APPEND _doxygen_input ${PROJECT_SOURCE_DIR}/README.md ${_doxygen_examples})
    string(REPLACE ";" " " DOXYGEN_INPUT "${_doxygen_input}")

    string(REPLACE ";" " " DOXYGEN_EXAMPLE_PATH "${_doxygen_example_path}")

    configure_file(Doxyfile.in Doxyfile @ONLY)

    add_custom_command(
        OUTPUT ${_doxygen_output_file}
        DEPENDS ${_doxygen_input} "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${_doxygen_output_dir}"
        COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
        COMMAND ${CMAKE_COMMAND} -E touch_nocreate "${_doxygen_output_file}"
        COMMENT "Generating API documentation using Doxygen"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    add_custom_target(doc ALL
        DEPENDS "${_doxygen_output_file}"
        SOURCES ${_doxygen_input} ${_doxygen_example_path} Doxyfile.in
    )
    set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES html)

    install(
        DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html"
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
        COMPONENT ${PROJECT_NAME}-doc
    )

    if(${PROJECT_NAME}_DEVELOPER AND TARGET Git::git AND EXISTS "${CMAKE_SOURCE_DIR}/.git/config")
        set(_ghpages_dir "${CMAKE_CURRENT_BINARY_DIR}/gh-pages")
        set(_ghpages_output "${_ghpages_dir}/.git/config")
        add_custom_command(
            OUTPUT "${_ghpages_output}"
            DEPENDS "${_doxygen_output_file}"
            COMMAND ${CMAKE_COMMAND} -E remove_directory "${_ghpages_dir}"
            COMMAND Git::git clone -b gh-pages "${CMAKE_SOURCE_DIR}/.git" "${_ghpages_dir}"
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${_doxygen_output_dir}" "${_ghpages_dir}"
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


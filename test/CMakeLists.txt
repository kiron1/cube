#=============================================================================
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt
#=============================================================================

set(_tests
    test_globject.cpp
    test_mat.cpp
)

foreach(_test IN LISTS _tests)
    get_filename_component(_target "${_test}" NAME_WE)
    add_executable(${_target} ${_test})
    target_include_directories(${_target}
        PRIVATE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
            .
    )
    target_compile_definitions(${_target}
        PRIVATE
          CATCH_CONFIG_MAIN=1
    )
    target_link_libraries(${_target}
        PRIVATE
          ${PROJECT_NAME}::gloo
    )
    add_test(NAME ${_target} COMMAND $<TARGET_FILE:${_target}>)
endforeach()


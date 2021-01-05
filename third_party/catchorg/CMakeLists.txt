#=============================================================================
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt
#=============================================================================

add_library(catch2 INTERFACE)
target_include_directories(catch2
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>

)

add_library(catch2_with_main INTERFACE)
target_compile_definitions(catch2_with_main
    INTERFACE
        CATCH_CONFIG_MAIN=1
)
target_link_libraries(catch2_with_main
    INTERFACE
        catch2
)


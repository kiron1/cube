#=============================================================================
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt
#=============================================================================

add_library(catch INTERFACE catch.hpp)
target_include_directories(catch
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>

)
target_compile_definitions(catch
    INTERFACE
        CATCH_CONFIG_MAIN=1
)
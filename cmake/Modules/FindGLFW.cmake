#.rst:
# FindGLFW  
# -------------
#
# FindModule for GLFW.
#
# IMPORTED Targets
# ^^^^^^^^^^^^^^^^
#
# ``GLFW::GLFW``
#  Defined if the system has GLFW.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module sets the following variables:
#
# ``GLFW_FOUND``
#  True, if the system has clang-format.
#

find_path(GLFW_INCLUDE_DIR
    NAMES GLFW/glfw3.h
    PATH_SUFFIXES include
    PATHS ENV GLFW_ROOT
)
mark_as_advanced(GLFW_INCLUDE_DIR)

set(GLFW_INCLUDE_DIRS ${GLFW_INCLUDE_DIR})

find_library(GLFW_LIBRARY_RELEASE
    NAMES glfw3
    NAMES_PER_DIR
    PATH_SUFFIXES lib lib64 libx32 lib-macos
    PATHS ENV GLFW_ROOT
)

find_library(GLFW_LIBRARY_DEBUG
    NAMES glfw3d
    NAMES_PER_DIR
    PATH_SUFFIXES lib lib64 lib-macos
    PATHS ENV GLFW_ROOT
)

include(SelectLibraryConfigurations)
select_library_configurations(GLFW)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLFW
    FOUND_VAR GLFW_FOUND
    REQUIRED_VARS GLFW_INCLUDE_DIR GLFW_LIBRARIES
)

if(APPLE)
   find_library(APPKIT_LIBRARY AppKit)
   find_library(CARBON_LIBRARY Carbon)
   find_library(IOKIT_LIBRARY IOKit)
   mark_as_advanced(APPKIT_LIBRARY CARBON_LIBRARY  IOKIT_LIBRARY)
endif()


if(NOT TARGET GLFW::GLFW)
    add_library(GLFW::GLFW UNKNOWN IMPORTED)
    set_target_properties(GLFW::GLFW
        PROPERTIES
              INTERFACE_INCLUDE_DIRECTORIES "${GLFW_INCLUDE_DIRS}"
              IMPORTED_LOCATION_RELEASE "${GLFW_LIBRARY_RELEASE}"
              IMPORTED_LOCATION_DEBUG "${GLFW_LIBRARY_DEBUG}"
              IMPORTED_CONFIGURATIONS "RELEASE;DEBUG"
    )
    if(APPLE)
        set_target_properties(GLFW::GLFW
            PROPERTIES
                INTERFACE_LINK_LIBRARIES "${APPKIT_LIBRARY};${CARBON_LIBRARY};${IOKIT_LIBRARY}"
        )
    endif()
endif()

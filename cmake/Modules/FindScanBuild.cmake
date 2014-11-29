#.rst:
# FindScanBuild
# -------------
#
# FindModule for LLVM scan-build script.
#
# IMPORTED Targets
# ^^^^^^^^^^^^^^^^
#
# ``ScanBuild::scan-build``
#  Defined if the system has the scan-build script.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module sets the following variables:
#
# ``ScanBuild_FOUND``
#  True, if the system has scan-build.
# ``ScanBuild_EXECUTABLE``
#  Path to the scan-build script.
#
# Functiosn
# ^^^^^^^^^
#
# ``add_scan_build(target)``
#  If no function with the name ``add_scan_build`` is present this
#  function will be added.
#  This function adds a new target which runs the scan-build command
#  against the current CMake project.
#  The function expects a target name as the only parameter.
#  Target properties can be used to customize the behaviour.
#
#  Properties used during CMake configureation step:
#   - SCANBUILD_BUILD_TYPE - Specify CMAKE_BUILD_TYPE.
#   - SCANBUILD_INITIAL_CACHE - Use initial chache file (cmake -C argument).
#   - SCANBUILD_CCC_ANALYZER_EXECUTABLE (advanced)
#   - SCANBUILD_CXX_ANALYZER_EXECUTABLE (advanced)
#
#  Properties used during scan-build step:
#   - SCANBUILD_TITLE - Customize the HTML title of the report.
#   - SCANBUILD_STATUS_BUGS - Fail target when reports are found.
#   - SCANBUILD_C_COMPILER (advanced)
#   - SCANBUILD_CXX_COMPILER (advanced)
#


find_program(ScanBuild_EXECUTABLE
        DOC "Path to clang's scan-build executable."
        NAMES scan-build scan-build-3.7 scan-build-3.6 scan-build-3.5 scan-build-3.4
)

if(ScanBuild_EXECUTABLE)
    get_filename_component(_scanbuild_root "${ScanBuild_EXECUTABLE}" REALPATH)
    get_filename_component(_scanbuild_root "${_scanbuild_root}" DIRECTORY)

    find_program(CCC_ANALYZER_EXECUTABLE
        DOC "Path to the ccc-analyzer exectuable distributed with scan-build."
        NAMES ccc-analyzer
        PATHS "${_scanbuild_root}"
    )
    find_program(CXX_ANALYZER_EXECUTABLE
        DOC "Path to the cxx-analyzer exectuable distributed with scan-build."
        NAMES c++-analyzer
        PATHS "${_scanbuild_root}"
    )
    if(CCC_ANALYZER_EXECUTABLE AND CXX_ANALYZER_EXECUTABLE)
        add_executable(ScanBuild::scan-build IMPORTED)
        set_target_properties(ScanBuild::scan-build PROPERTIES
            IMPORTED_LOCATION "${ScanBuild_EXECUTABLE}"
        )
    endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ScanBuild
    FOUND_VAR ScanBuild_FOUND
    REQUIRED_VARS ScanBuild_EXECUTABLE CCC_ANALYZER_EXECUTABLE CXX_ANALYZER_EXECUTABLE
)

if(TARGET ScanBuild::scan-build AND NOT COMMAND add_scan_build)
    function(add_scan_build _target)
        set(_builddir "${CMAKE_CURRENT_BINARY_DIR}/${_target}/build")
        set(_resultdir "${CMAKE_CURRENT_BINARY_DIR}/${_target}/result")
        set(_cfgargs
            "-H${CMAKE_SOURCE_DIR}"
            "-B${_builddir}"
            "-G${CMAKE_GENERATOR}"
            "$<$<BOOL:$<TARGET_PROPERTY:${_target},SCANBUILD_INITIAL_CACHE>>:-C$<TARGET_PROPERTY:${_target},SCANBUILD_INITIAL_CACHE>>"
            "-DCMAKE_C_COMPILER=$<TARGET_PROPERTY:${_target},SCANBUILD_CCC_ANALYZER_EXECUTABLE>"
            "-DCMAKE_CXX_COMPILER=$<TARGET_PROPERTY:${_target},SCANBUILD_CXX_ANALYZER_EXECUTABLE>"
            "$<$<BOOL:$<TARGET_PROPERTY:${_target},SCANBUILD_BUILD_TYPE>>:-DCMAKE_BUILD_TYPE=$<TARGET_PROPERTY:${_target},SCANBUILD_BUILD_TYPE>>"
            "$<$<BOOL:$<TARGET_PROPERTY:${_target},SCANBUILD_DEFINES>>:-D$<JOIN:$<TARGET_PROPERTY:${_target},SCANBUILD_DEFINES>, -D>>"
        )
        set(_scanargs
            "-o" "${_resultdir}"
            "--use-cc=$<TARGET_PROPERTY:${_target},SCANBUILD_C_COMPILER>"
            "--use-c++=$<TARGET_PROPERTY:${_target},SCANBUILD_CXX_COMPILER>"
            "$<$<BOOL:$<TARGET_PROPERTY:${_target},SCANBUILD_TITLE>>:--html-title=$<TARGET_PROPERTY:${_target},SCANBUILD_TITLE>>"
            "$<$<BOOL:$<TARGET_PROPERTY:${_target},SCANBUILD_STATUS_BUGS>>:--status-bugs>"
        )
        set(_buildcmd
            ${CMAKE_COMMAND}
            --build
            "${_builddir}"
            "$<$<BOOL:$<TARGET_PROPERTY:${_target},SCANBUILD_BUILD_TYPE>>:--config $<TARGET_PROPERTY:${_target},SCANBUILD_BUILD_TYPE>>"
        )
        add_custom_target(${_target}
            COMMAND ${CMAKE_COMMAND} -E remove_directory "${_builddir}"
            COMMAND ${CMAKE_COMMAND} ${_cfgargs}
            COMMAND ScanBuild::scan-build ${_scanargs} ${_buildcmd}
        )
        set_target_properties(${_target} PROPERTIES
            SCANBUILD_CCC_ANALYZER_EXECUTABLE "${CCC_ANALYZER_EXECUTABLE}"
            SCANBUILD_CXX_ANALYZER_EXECUTABLE "${CXX_ANALYZER_EXECUTABLE}"
            SCANBUILD_C_COMPILER "${CMAKE_C_COMPILER}"
            SCANBUILD_CXX_COMPILER "${CMAKE_CXX_COMPILER}"
            SCANBUILD_INITIAL_CACHE "${${_target}_INITIAL_CACHE}"
        )
    endfunction()
endif()


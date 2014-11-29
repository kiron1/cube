#.rst:
# FindClangFormat
# -------------
#
# FindModule for LLVM clang-format script.
#
# IMPORTED Targets
# ^^^^^^^^^^^^^^^^
#
# ``ClangFormat::clang-format``
#  Defined if the system has the clang-format script.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module sets the following variables:
#
# ``ClangFormat_FOUND``
#  True, if the system has clang-format.
# ``ClangFormat_EXECUTABLE``
#  Path to the clang-format script.
#


find_program(ClangFormat_EXECUTABLE
    DOC "Path to the clang-format executable."
    NAMES clang-format clang-format-3.7 clang-format-3.6 clang-format-3.5 clang-format 3.4
)

if(ClangFormat_EXECUTABLE)
    add_executable(ClangFormat::clang-format IMPORTED)
    set_target_properties(ClangFormat::clang-format PROPERTIES
        IMPORTED_LOCATION "${ClangFormat_EXECUTABLE}"
    )

    execute_process(COMMAND ${ClangFormat_EXECUTABLE} --version
        OUTPUT_VARIABLE _cformat_version
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(_cformat_version MATCHES ".*clang-format version ([0-9]+.[0-9]+.[0-9]+)")
        set(ClangFormat_VERSION_STRING "${CMAKE_MATCH_1}")
    endif()
    unset(_cformat_version)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ClangFormat
    FOUND_VAR ClangFormat_FOUND
    VERSION_VAR ClangFormat_VERSION_STRING
    REQUIRED_VARS ClangFormat_EXECUTABLE
)

if(TARGET ClangFormat::clang-format AND NOT COMMAND add_clang_format)
    function(add_clang_format _target)
        include(CMakeParseArguments)
        cmake_parse_arguments(_arg "" "STYLE" "SOURCES;TARGETS" ${ARGN})
        set(_formatargs -i)
        if(_arg_STYLE)
                list(APPEND _formatargs "--style=${_arg_STYLE}")
        endif()
        set(_src ${_arg_SOURCES})
        foreach(_target IN LISTS _arg_TARGETS)
            get_target_property(_files ${_target} SOURCES)
            if(_files)
                list(APPEND _src ${_files})
            endif()
        endforeach()

        add_custom_target(${_target}
            COMMAND ClangFormat::clang-format ${_formatargs} ${_src}
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        )
    endfunction()
endif()


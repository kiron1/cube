#.rst:
# FindBoostQuickbook
# ------------------
#
# FindModule for Boost quickbook tools.
#
# IMPORTED Targets
# ^^^^^^^^^^^^^^^^
#
# ``Boost::quickbook``
#  Defined if the system has Boost's quickbook executable.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module sets the following variables:
#
# ``BoostQuickbook_FOUND``
#  True, if the system has quickbook.
# ``BoostQuickbook_EXECUTABLE``
#  Path to the quickbook exectuable.
#


find_program(BoostQuickbook_EXECUTABLE
        DOC "Path to the quickbook executable."
        NAMES quickbook
        PATH ${BOOST_ROOT} ENV BOOST_ROOT
        PATH_SUFFIXES bin dist/bin
)
mark_as_advanced(BoostQuickbook_EXECUTABLE)

if(BoostQuickbook_EXECUTABLE)
	add_executable(Boost::quickbook IMPORTED)
	set_target_properties(Boost::quickbook PROPERTIES
        IMPORTED_LOCATION "${BoostQuickbook_EXECUTABLE}"
	)

	execute_process(COMMAND ${BoostQuickbook_EXECUTABLE} --version
		OUTPUT_VARIABLE BoostQuickbook_VERSION_STRING
		ERROR_QUIET
		OUTPUT_STRIP_TRAILING_WHITESPACE
    )
	if (_quickbook_version MATCHES "^Quickbook Version ([0-9]+\\.[0-9]+\\.[0-9]+).*")
        set(BoostQuickbook_VERSION_STRING "${CMAKE_MATCH_1}")
    else()
	    unset(BoostQuickbook_VERSION_STRING)
	endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(BoostQuickbook
	VERSION_VAR BoostQuickbook_VERSION_STRING
    REQUIRED_VARS BoostQuickbook_EXECUTABLE
)

if(BoostQuickbook_EXECUTABLE AND NOT COMMAND quickbook_process)
    function(quickbook_process _output)
        include(CMakeParseArguments)
        cmake_parse_arguments(_arg
            ""
            "INPUT;WORKING_DIRECTORY"
            "DEPENDS"
            ${ARGN}
        )
        get_filename_component(_absfile "${_arg_INPUT}" ABSOLUTE)
        get_filename_component(_output "${_output}" ABSOLUTE)
        add_custom_command(OUTPUT "${_output}"
            DEPENDS "${_absfile}" ${_arg_DEPENDS}
            COMMAND Boost::quickbook
                --xinclude-base "${CMAKE_CURRENT_SOURCE_DIR}"
                --input-file "${_absfile}"
                --output-file "${_output}"
            WORKING_DIRECTORY "${_arg_WORKING_DIRECTORY}"
        )
    endfunction()
endif()


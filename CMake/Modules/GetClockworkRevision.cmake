

# Get Clockwork library revision number

if (DEFINED ENV{TRAVIS_COMMIT})
    # Use the same commit-ish used by CI server to describe the repository
    set (ARG $ENV{TRAVIS_COMMIT})
else ()
    set (ARG --dirty)
endif ()
execute_process (COMMAND git describe ${ARG} RESULT_VARIABLE GIT_EXIT_CODE OUTPUT_VARIABLE LIB_REVISION ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
if (NOT GIT_EXIT_CODE EQUAL 0)
    # No GIT command line tool or not a GIT repository
    set (LIB_REVISION Unversioned)
endif ()
if (FILENAME)
    # Output complete revision number to a file
    file (WRITE ${FILENAME} "const char* revision=\"${LIB_REVISION}\";\n")
else ()
    # Output just major.minor.patch number to stdout
    string (REGEX MATCH "[^.]+\\.[^-]+" VERSION ${LIB_REVISION})            # Assume release tag always has major.minor format
    if (VERSION)
        string (REGEX MATCH "${VERSION}-([^-]+)" PATCH ${LIB_REVISION})     # Subsequent commits count after a release tag is treated as patch number
        if (PATCH)
            set (VERSION ${VERSION}.${CMAKE_MATCH_1})
        endif ()
    else ()
        set (VERSION 0.0)
    endif ()
    execute_process (COMMAND ${CMAKE_COMMAND} -E echo ${VERSION})
endif ()

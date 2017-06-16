# Recursively retrieve all the header files in our include directory

FILE(GLOB_RECURSE HEADER_FILES ${INCLUDE_DIR}/*.h)

# Make sure that all of our header files have their actual contents

FOREACH(HEADER_FILE ${HEADER_FILES})
    # Read the contents of our header file and strip any leading/trailing spaces

    FILE(READ ${HEADER_FILE} HEADER_FILE_CONTENTS)

    STRING(STRIP ${HEADER_FILE_CONTENTS} HEADER_FILE_CONTENTS)

    # Retrieve the directory of our header file and determine the relative path
    # from it to our QtWebKit source directory, and account for the fact that on
    # Windows we are one folder down too many (because we deploy our files under
    # a 'release' or 'debug' folder)

    GET_FILENAME_COMPONENT(HEADER_FILE_DIR ${HEADER_FILE} DIRECTORY)

    IF(WIN32)
        SET(UP_DIR /..)
    ENDIF()

    FILE(RELATIVE_PATH RELATIVE_PATH ${HEADER_FILE_DIR}${UP_DIR} ${SOURCE_DIR})

    # Check whether the contents of our header file is something like
    #     #include "<relative_path>...<header_file_name>"
    # and, if so, replace its contents with that of the included file, i.e. the
    # actual contents of our header file

    GET_FILENAME_COMPONENT(HEADER_FILE_NAME ${HEADER_FILE} NAME)

    STRING(REGEX MATCH "^#include \"${RELATIVE_PATH}.*${HEADER_FILE_NAME}\"$" INCLUDE_SOURCE_HEADER_FILE ${HEADER_FILE_CONTENTS})

    IF(NOT "${INCLUDE_SOURCE_HEADER_FILE}" STREQUAL "")
        STRING(REGEX REPLACE "^#include \"" "" HEADER_FILE_RELATIVE_PATH ${INCLUDE_SOURCE_HEADER_FILE})
        STRING(REGEX REPLACE "\"$" "" HEADER_FILE_RELATIVE_PATH ${HEADER_FILE_RELATIVE_PATH})

        IF(WIN32)
            SET(UP_DIR ../)
        ENDIF()

        EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy ${UP_DIR}${HEADER_FILE_RELATIVE_PATH} ${HEADER_FILE_NAME}
                        WORKING_DIRECTORY ${HEADER_FILE_DIR})
    ENDIF()
ENDFOREACH()

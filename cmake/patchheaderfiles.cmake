# Recursively retrieve all the header files in our include directory

FILE(GLOB_RECURSE HEADER_FILES ${INCLUDE_DIR}/*.h)

# Make sure that all of our header files have their actual contents

FOREACH(HEADER_FILE ${HEADER_FILES})
    # Read the contents of our header file and strip any leading/trailing spaces

    FILE(READ ${HEADER_FILE} HEADER_FILE_CONTENTS)

    STRING(STRIP ${HEADER_FILE_CONTENTS} HEADER_FILE_CONTENTS)

    # Retrieve the directory of our header file and determine the relative path
    # from it to our Qt package's source directory

    GET_FILENAME_COMPONENT(HEADER_FILE_DIR ${HEADER_FILE} DIRECTORY)

    FILE(RELATIVE_PATH RELATIVE_PATH ${HEADER_FILE_DIR} ${SOURCE_DIR})

    # Check whether the contents of our header file is something like
    #     #include "<relative_path>...<header_file_name>"
    # and, if so, replace its contents with that of the included file, i.e. the
    # actual contents of our header file

    GET_FILENAME_COMPONENT(HEADER_FILE_NAME ${HEADER_FILE} NAME)

    STRING(REGEX MATCH "^#include \"${RELATIVE_PATH}.*${HEADER_FILE_NAME}\"$" INCLUDE_SOURCE_HEADER_FILE ${HEADER_FILE_CONTENTS})

    IF(NOT "${INCLUDE_SOURCE_HEADER_FILE}" STREQUAL "")
        STRING(REGEX REPLACE "^#include \"" "" HEADER_FILE_RELATIVE_PATH ${INCLUDE_SOURCE_HEADER_FILE})
        STRING(REGEX REPLACE "\"$" "" HEADER_FILE_RELATIVE_PATH ${HEADER_FILE_RELATIVE_PATH})

        EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy ${HEADER_FILE_RELATIVE_PATH} ${HEADER_FILE_NAME}
                        WORKING_DIRECTORY ${HEADER_FILE_DIR})
    ENDIF()
ENDFOREACH()

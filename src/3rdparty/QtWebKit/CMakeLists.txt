MACRO(RETRIEVE_QT_WEBKIT_LIBRARY WEBKIT_NAME SHA1_VALUE)
    # Create, if needed, the folder where the Qt WebKit library is to be located

    SET(QT_WEBKIT_LIBRARIES_DIR ${QT_WEBKIT_DIR}/lib)

    IF(NOT EXISTS ${QT_WEBKIT_LIBRARIES_DIR})
        FILE(MAKE_DIRECTORY ${QT_WEBKIT_LIBRARIES_DIR})
    ENDIF()

    # Make sure that the Qt WebKit library, if it exists, has the expected
    # SHA-1 value

    SET(REAL_DIRNAME ${QT_WEBKIT_LIBRARIES_DIR}/Qt${WEBKIT_NAME}.framework)
    SET(REAL_FILENAME ${REAL_DIRNAME}/Versions/5/Qt${WEBKIT_NAME})

    IF(EXISTS ${REAL_FILENAME})
        FILE(SHA1 ${REAL_FILENAME} REAL_SHA1_VALUE)

        IF(NOT "${REAL_SHA1_VALUE}" STREQUAL "${SHA1_VALUE}")
            # The file doesn't have the expected SHA-1 value, so remove the Qt
            # WebKit library

            FILE(REMOVE_RECURSE ${REAL_DIRNAME})
        ENDIF()
    ENDIF()

    # Retrieve the Qt WebKit library from the OpenCOR website, if needed

    IF(NOT EXISTS ${REAL_FILENAME})
        IF(SHOW_INFORMATION_MESSAGE)
            MESSAGE("Retrieving Qt${WEBKIT_NAME}...")
        ENDIF()

        # Retrieve the compressed version of the Qt WebKit library

        SET(COMPRESSED_FILENAME Qt${WEBKIT_NAME}.tar.gz)
        SET(REAL_COMPRESSED_FILENAME ${QT_WEBKIT_LIBRARIES_DIR}/${COMPRESSED_FILENAME})

        FILE(DOWNLOAD "http://www.opencor.ws/binaries/src/3rdparty/QtWebKit/${PLATFORM_DIR}/${COMPRESSED_FILENAME}" ${REAL_COMPRESSED_FILENAME}
             SHOW_PROGRESS STATUS STATUS)

        # Uncompress the compressed version of the Qt WebKit library, should we
        # have managed to retrieve it

        LIST(GET STATUS 0 STATUS_CODE)

        IF(${STATUS_CODE} EQUAL 0)
            EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E tar -xzf ${REAL_COMPRESSED_FILENAME}
                            WORKING_DIRECTORY ${QT_WEBKIT_LIBRARIES_DIR} OUTPUT_QUIET)
            FILE(REMOVE ${REAL_COMPRESSED_FILENAME})
        ELSE()
            FILE(REMOVE ${REAL_COMPRESSED_FILENAME})
            # Note: this is in case we had an HTTP error of sorts, in which case
            #       we would end up with an empty file...

            MESSAGE(FATAL_ERROR "The compressed version of Qt${WEBKIT_NAME} could not be retrieved...")
        ENDIF()

        # Check that the Qt WebKit library, if we managed to retrieve it, has
        # the expected SHA-1 value

        IF(EXISTS ${REAL_FILENAME})
            FILE(SHA1 ${REAL_FILENAME} REAL_SHA1_VALUE)

            IF(NOT "${REAL_SHA1_VALUE}" STREQUAL "${SHA1_VALUE}")
                FILE(REMOVE_RECURSE ${REAL_DIRNAME})

                MESSAGE(FATAL_ERROR "Qt${WEBKIT_NAME} does not have the expected SHA-1 value...")
            ENDIF()
        ELSE()
            FILE(REMOVE ${REAL_COMPRESSED_FILENAME})

            MESSAGE(FATAL_ERROR "Qt${WEBKIT_NAME} could not be uncompressed...")
        ENDIF()
    ENDIF()
ENDMACRO()

# Make sure that Qt WebKit's CMake files can be found

SET(QT_WEBKIT_DIR ${CMAKE_SOURCE_DIR}/src/3rdparty/QtWebKit)

SET(Qt5WebKit_DIR ${QT_WEBKIT_DIR}/cmake/${PLATFORM_DIR})
SET(Qt5WebKitWidgets_DIR ${QT_WEBKIT_DIR}/cmake/${PLATFORM_DIR})

# Retrieve our two Qt WebKit libraries

RETRIEVE_QT_WEBKIT_LIBRARY(WebKit        5b4b5b3498c18352dba700636bfbc03f2961f9e9)
RETRIEVE_QT_WEBKIT_LIBRARY(WebKitWidgets 587045ad5a47e384ac02f0d598c7d79af1bc2cfc)

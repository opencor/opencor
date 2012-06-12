# Files that make up QtSingleApplication

IF(WIN32)
    SET(PLATFORM win)
ELSE()
    SET(PLATFORM unix)
ENDIF()

SET(SOURCES ${SOURCES}
    ${MAIN_PROJECT_SOURCE_DIR}/src/3rdparty/QtSingleApplication/src/qtlocalpeer.cpp
    ${MAIN_PROJECT_SOURCE_DIR}/src/3rdparty/QtSingleApplication/src/qtlockedfile.cpp
    ${MAIN_PROJECT_SOURCE_DIR}/src/3rdparty/QtSingleApplication/src/qtlockedfile_${PLATFORM}.cpp
    ${MAIN_PROJECT_SOURCE_DIR}/src/3rdparty/QtSingleApplication/src/qtsingleapplication.cpp
)

SET(HEADERS_MOC ${HEADERS_MOC}
    ${MAIN_PROJECT_SOURCE_DIR}/src/3rdparty/QtSingleApplication/src/qtlocalpeer.h
    ${MAIN_PROJECT_SOURCE_DIR}/src/3rdparty/QtSingleApplication/src/qtsingleapplication.h
)

# Include directory

INCLUDE_DIRECTORIES(${MAIN_PROJECT_SOURCE_DIR}/src/3rdparty/QtSingleApplication/src)

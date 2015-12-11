# Files that make up QtSingleApplication

IF(WIN32)
    SET(PLATFORM_SPECIFIC_CPP qtlockedfile_win.cpp)
ELSE()
    SET(PLATFORM_SPECIFIC_CPP qtlockedfile_unix.cpp)
ENDIF()

SET(SOURCES ${SOURCES}
    src/3rdparty/QtSingleApplication/src/qtlocalpeer.cpp
    src/3rdparty/QtSingleApplication/src/qtlockedfile.cpp
    src/3rdparty/QtSingleApplication/src/${PLATFORM_SPECIFIC_CPP}
    src/3rdparty/QtSingleApplication/src/qtsingleapplication.cpp
)

SET(HEADERS_MOC ${HEADERS_MOC}
    src/3rdparty/QtSingleApplication/src/qtlocalpeer.h
    src/3rdparty/QtSingleApplication/src/qtsingleapplication.h
)

# Include directory

INCLUDE_DIRECTORIES(src/3rdparty/QtSingleApplication/src)

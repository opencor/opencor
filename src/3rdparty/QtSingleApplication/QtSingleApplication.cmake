# Files that make up QtSingleApplication

SET(SOURCES ${SOURCES}
    src/3rdparty/QtSingleApplication/src/qtlocalpeer.cpp
    src/3rdparty/QtSingleApplication/src/qtsingleapplication.cpp
)

SET(HEADERS_MOC ${HEADERS_MOC}
    src/3rdparty/QtSingleApplication/src/qtlocalpeer.h
    src/3rdparty/QtSingleApplication/src/qtsingleapplication.h
)

# Include directory

INCLUDE_DIRECTORIES(src/3rdparty/QtSingleApplication/src)

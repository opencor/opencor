# Files that make up QtSingleApplication

set(SOURCES ${SOURCES}
    src/3rdparty/QtSingleApplication/src/qtlocalpeer.cpp
    src/3rdparty/QtSingleApplication/src/qtsingleapplication.cpp
)

set(HEADERS_MOC ${HEADERS_MOC}
    src/3rdparty/QtSingleApplication/src/qtlocalpeer.h
    src/3rdparty/QtSingleApplication/src/qtsingleapplication.h
)

# Include directory

include_directories(src/3rdparty/QtSingleApplication/src)

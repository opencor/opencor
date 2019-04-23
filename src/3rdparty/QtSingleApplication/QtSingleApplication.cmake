# Files that make up QtSingleApplication

set(QT_SINGLE_APPLICATION_DIR src/3rdparty/QtSingleApplication/src)

set(SOURCES ${SOURCES}
    ${QT_SINGLE_APPLICATION_DIR}/qtlocalpeer.cpp
    ${QT_SINGLE_APPLICATION_DIR}/qtsingleapplication.cpp
)

# Include directory

include_directories(${QT_SINGLE_APPLICATION_DIR})

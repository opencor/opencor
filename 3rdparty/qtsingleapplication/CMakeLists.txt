# Files that make up QtSingleApplication

SET(${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_SOURCES}
    ${3RDPARTYLIB_DIR}/qtlocalpeer.cpp
    ${3RDPARTYLIB_DIR}/qtsingleapplication.cpp
)

SET(${PROJECT_NAME}_HEADERS_MOC ${${PROJECT_NAME}_HEADERS_MOC}
    ${3RDPARTYLIB_DIR}/qtlocalpeer.h
    ${3RDPARTYLIB_DIR}/qtsingleapplication.h
)

PROJECT(QtSingleApplication)

# Files that make up QtSingleApplication

SET(SOURCES
    qtlocalpeer.cpp
    qtsingleapplication.cpp
)

SET(HEADERS_MOC
    qtlocalpeer.h
    qtsingleapplication.h
)

# Qt specific

FIND_PACKAGE(Qt4 4.6.0 REQUIRED)

INCLUDE(${QT_USE_FILE})

QT4_WRAP_CPP(SOURCES_MOC ${HEADERS_MOC})

# Rules to build QtSingleApplication

IF(APPLE)
    SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
ELSE()
    SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
ENDIF()
# Note: the above is only required so that we can quickly test OpenCOR without
#       first having to package everything

ADD_LIBRARY(${PROJECT_NAME} SHARED
    ${SOURCES}
    ${SOURCES_MOC}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
)

# Rules to package QtSingleApplication

IF(WIN32)
    INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
ELSEIF(NOT APPLE)
    INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
ENDIF()

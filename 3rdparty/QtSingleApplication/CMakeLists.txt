PROJECT(QtSingleApplication)

# Files that make up QtSingleApplication

SET(SOURCES
    src/qtlocalpeer.cpp
    src/qtsingleapplication.cpp
)

SET(HEADERS_MOC
    src/qtlocalpeer.h
    src/qtsingleapplication.h
)

# Various include directories

SET(LOCAL_THIRD_PARTY_LIBRARY_INCLUDE_DIRS src)
SET(THIRD_PARTY_LIBRARY_INCLUDE_DIRS ${LOCAL_THIRD_PARTY_LIBRARY_INCLUDE_DIRS} PARENT_SCOPE)

INCLUDE_DIRECTORIES(${LOCAL_THIRD_PARTY_LIBRARY_INCLUDE_DIRS})

# Rules to build QtSingleApplication

QT4_WRAP_CPP(SOURCES_MOC ${HEADERS_MOC})

IF(APPLE)
    SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
ELSE()
    SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
ENDIF()
# Note: the above is only required so that we can quickly test
#       QtSingleApplication without first having to package everything

ADD_LIBRARY(${PROJECT_NAME} SHARED
    ${SOURCES}
    ${SOURCES_MOC}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
)

# Linker settings

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${LINK_FLAGS_PROPERTIES}")

# Rules to package QtSingleApplication

IF(WIN32)
    INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
ELSEIF(NOT APPLE)
    INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
ENDIF()

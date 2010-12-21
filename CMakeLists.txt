project(OpenCOR)

cmake_minimum_required(VERSION 2.8)

# Version of OpenCOR

set(${PROJECT_NAME}_VERSION_MAJOR 0 CACHE STRING "${PROJECT_NAME} major version number")
set(${PROJECT_NAME}_VERSION_MINOR 1 CACHE STRING "${PROJECT_NAME} minor version number")
set(${PROJECT_NAME}_VERSION_PATCH 1 CACHE STRING "${PROJECT_NAME} build version number")
set(${PROJECT_NAME}_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH}")

# Files that make up OpenCOR

set(${PROJECT_NAME}_SOURCES
   src/main.cpp
   src/mainwindow.cpp
)

set(${PROJECT_NAME}_HEADERS
)

set(${PROJECT_NAME}_HEADERS_MOC
   src/mainwindow.h
)

set(${PROJECT_NAME}_UIS
   src/mainwindow.ui
)

set(${PROJECT_NAME}_RESOURCES
)

# Qt specific

find_package(Qt4 4.7 REQUIRED)

if(WIN32)
   set(QT_USE_QTMAIN TRUE)
   # Note: this ensures that winmain@16 is available on Windows
endif(WIN32)

include(${QT_USE_FILE})

QT4_WRAP_CPP(${PROJECT_NAME}_SOURCES_MOC ${${PROJECT_NAME}_HEADERS_MOC})
QT4_WRAP_UI(${PROJECT_NAME}_SOURCES_UIS ${${PROJECT_NAME}_UIS})
QT4_ADD_RESOURCES(${PROJECT_NAME}_SOURCES_RCS ${${PROJECT_NAME}_RESOURCES})

# Rules to build OpenCOR itself

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE
   ${${PROJECT_NAME}_SOURCES}
   ${${PROJECT_NAME}_SOURCES_MOC}
   ${${PROJECT_NAME}_SOURCES_UIS}
   ${${PROJECT_NAME}_SOURCES_RCS}
)

target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES})

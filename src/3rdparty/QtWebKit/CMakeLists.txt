# Make sure that Qt WebKit's CMake files can be found

SET(QT_WEBKIT_DIR ${CMAKE_SOURCE_DIR}/src/3rdparty/QtWebKit)

SET(Qt5WebKit_DIR ${QT_WEBKIT_DIR}/cmake/${PLATFORM_DIR})
SET(Qt5WebKitWidgets_DIR ${QT_WEBKIT_DIR}/cmake/${PLATFORM_DIR})

# Make sure that QtWebKit's CMake files can be found

SET(QT_WEBKIT_DIR ${CMAKE_SOURCE_DIR}/src/3rdparty/QtWebKit)

LIST(APPEND CMAKE_PREFIX_PATH ${QT_WEBKIT_DIR}/cmake/${PLATFORM_DIR}/Qt5WebKit)
LIST(APPEND CMAKE_PREFIX_PATH ${QT_WEBKIT_DIR}/cmake/${PLATFORM_DIR}/Qt5WebKitWidgets)

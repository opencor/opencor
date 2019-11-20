# If Qt has support for QtWebKit, add it:
qtHaveModule(webkit):CONFIG += PythonQtWebKit

CONFIG += qt

HEADERS += $$PWD/PythonQt_QtAll.h

SOURCES += $$PWD/PythonQt_QtAll.cpp

DEFINES += PYTHONQT_WITH_CORE
include ($$PYTHONQT_GENERATED_PATH/com_trolltech_qt_core/com_trolltech_qt_core.pri)

QT += gui printsupport svg widgets
DEFINES += PYTHONQT_WITH_GUI PYTHONQT_WITH_SVG
include ($$PYTHONQT_GENERATED_PATH/com_trolltech_qt_gui/com_trolltech_qt_gui.pri)
include ($$PYTHONQT_GENERATED_PATH/com_trolltech_qt_svg/com_trolltech_qt_svg.pri)

PythonQtWebKit {
  QT += webkit webkitwidgets
  DEFINES += PYTHONQT_WITH_WEBKIT
  include ($$PYTHONQT_GENERATED_PATH/com_trolltech_qt_webkit/com_trolltech_qt_webkit.pri)
}

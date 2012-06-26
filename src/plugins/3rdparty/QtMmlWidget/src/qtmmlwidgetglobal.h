#ifndef QTMMLWIDGETGLOBAL_H
#define QTMMLWIDGETGLOBAL_H

#ifdef _WIN32
    #ifdef QtMmlWidget_PLUGIN
        #define QTMMLWIDGET_EXPORT __declspec(dllexport)
    #else
        #define QTMMLWIDGET_EXPORT __declspec(dllimport)
    #endif
#else
    #define QTMMLWIDGET_EXPORT
#endif

#endif

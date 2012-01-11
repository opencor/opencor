#ifndef COMPUTERGLOBAL_H
#define COMPUTERGLOBAL_H

#ifdef OpenCOR_MAIN
    #define COMPUTER_EXPORT
#else
    #include <QObject>

    #ifdef Computer_PLUGIN
        #define COMPUTER_EXPORT Q_DECL_EXPORT
    #else
        #define COMPUTER_EXPORT Q_DECL_IMPORT
    #endif
#endif

#endif

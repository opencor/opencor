#ifndef Q_WS_WIN
    #include "cda_config_windows.h"
#else
    #ifdef Q_WS_MAC
        #include "cda_config_macosx.h"
    #else
        // Linux

        #include "cda_config_linux.h"
    #endif
#endif

#ifndef COMPUTERGLOBAL_H
#define COMPUTERGLOBAL_H

#ifdef _WIN32
    #ifdef Computer_PLUGIN
        #define COMPUTER_EXPORT __declspec(dllexport)
    #else
        #define COMPUTER_EXPORT __declspec(dllimport)
    #endif
#else
    #define COMPUTER_EXPORT
#endif

#endif

#ifndef COMPUTERGLOBAL_H
#define COMPUTERGLOBAL_H

#ifdef Computer_PLUGIN
    #define COMPUTER_EXPORT __declspec(dllexport)
#else
    #define COMPUTER_EXPORT __declspec(dllimport)
#endif

#endif

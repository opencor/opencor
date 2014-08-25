#ifndef COREDATASTOREGLOBAL_H
#define COREDATASTOREGLOBAL_H

#ifdef _WIN32
    #ifdef CoreDataStore_PLUGIN
        #define COREDATASTORE_EXPORT __declspec(dllexport)
    #else
        #define COREDATASTORE_EXPORT __declspec(dllimport)
    #endif
#else
    #define COREDATASTORE_EXPORT
#endif

#endif

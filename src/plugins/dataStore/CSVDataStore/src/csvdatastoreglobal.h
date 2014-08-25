#ifndef CSVDATASTOREGLOBAL_H
#define CSVDATASTOREGLOBAL_H

#ifdef _WIN32
    #ifdef CSVDataStore_PLUGIN
        #define CSVDATASTORE_EXPORT __declspec(dllexport)
    #else
        #define CSVDATASTORE_EXPORT __declspec(dllimport)
    #endif
#else
    #define CSVDATASTORE_EXPORT
#endif

#endif

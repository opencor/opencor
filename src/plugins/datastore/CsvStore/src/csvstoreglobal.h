#ifndef CSVSTOREGLOBAL_H
#define CSVSTOREGLOBAL_H

#ifdef _WIN32
    #ifdef CsvStore_PLUGIN
        #define CSVSTORE_EXPORT __declspec(dllexport)
    #else
        #define CSVSTORE_EXPORT __declspec(dllimport)
    #endif
#else
    #define CSVSTORE_EXPORT
#endif

#endif

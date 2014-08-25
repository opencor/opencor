#ifndef COREDATAGLOBAL_H
#define COREDATAGLOBAL_H

#ifdef _WIN32
    #ifdef CoreData_PLUGIN
        #define COREDATA_EXPORT __declspec(dllexport)
    #else
        #define COREDATA_EXPORT __declspec(dllimport)
    #endif
#else
    #define COREDATA_EXPORT
#endif

#endif

#ifndef COREGLOBAL_H
#define COREGLOBAL_H

#ifdef Core_PLUGIN
    #define CORE_EXPORT Q_DECL_EXPORT
#else
    #define CORE_EXPORT Q_DECL_IMPORT
#endif

#endif

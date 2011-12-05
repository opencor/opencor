#ifndef CELLMLMODELGLOBAL_H
#define CELLMLMODELGLOBAL_H

#ifdef OpenCOR_MAIN
    #define CELLMLMODEL_EXPORT
#else
    #ifdef CellMLModel_PLUGIN
        #define CELLMLMODEL_EXPORT Q_DECL_EXPORT
    #else
        #define CELLMLMODEL_EXPORT Q_DECL_IMPORT
    #endif
#endif

#endif

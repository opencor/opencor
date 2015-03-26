#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual void * dataStoreExporterInstance() const PURE;

    virtual QString dataStoreName() const PURE;

#undef PURE

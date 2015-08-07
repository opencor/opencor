#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual void * newDataStoreExporterInstance(const QString &pId = QString()) const PURE;
    virtual void deleteDataStoreExporterInstance(const QString &pId = QString()) const PURE;

    virtual QString dataStoreName() const PURE;

#undef PURE

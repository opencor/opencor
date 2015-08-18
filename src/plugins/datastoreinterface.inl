#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual DataStore::DataStoreExporter * newDataStoreExporterInstance(const QString &pId = QString()) const PURE;
    virtual void deleteDataStoreExporterInstance(DataStore::DataStoreExporter *pDataStoreExporterInstance) PURE;

    virtual QString dataStoreName() const PURE;

#undef PURE

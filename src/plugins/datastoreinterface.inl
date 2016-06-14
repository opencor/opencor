#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual QString dataStoreName() const PURE;

    virtual DataStore::DataStoreData * getData(const QString &pFileName,
                                               DataStore::DataStore *pDataStore) const PURE;

    virtual DataStore::DataStoreExporter * dataStoreExporterInstance(const QString &pFileName,
                                                                     DataStore::DataStore *pDataStore,
                                                                     DataStore::DataStoreData *pDataStoreData) const PURE;

#undef PURE

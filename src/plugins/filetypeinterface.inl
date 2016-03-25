#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual bool isFile(const QString &pFileName) const PURE;

    virtual QString fileTypeDescription(const QString &pMimeType) const PURE;

#undef PURE

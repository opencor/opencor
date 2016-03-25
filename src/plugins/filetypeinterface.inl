#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual QString fileTypeDescription(const QString &pMimeType) const PURE;

#undef PURE

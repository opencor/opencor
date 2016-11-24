#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual bool isFile(const QString &pFileName) const PURE;

    virtual QString mimeType() const PURE;
    virtual QString fileExtension() const PURE;
    virtual QString fileTypeDescription() const PURE;

    virtual QStringList fileTypeDefaultViews() const PURE;

#undef PURE

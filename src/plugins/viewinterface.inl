#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual Mode viewMode() const PURE;
    virtual QStringList viewMimeTypes(const MimeTypeMode &pMimeTypeMode) const PURE;
    virtual QString viewDefaultFileExtension() const PURE;

    virtual QWidget * viewWidget(const QString &pFileName) PURE;
    virtual void removeViewWidget(const QString &pFileName) PURE;
    virtual QString viewName() const PURE;

    virtual QIcon fileTabIcon(const QString &pFileName) const PURE;

#undef PURE

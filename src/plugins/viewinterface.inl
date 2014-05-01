#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual Mode viewMode() const PURE;
    virtual QStringList viewMimeTypes() const PURE;

    virtual void initializeView() PURE;
    virtual void finalizeView() PURE;

    virtual bool hasViewWidget(const QString &pFileName) PURE;
    virtual QWidget * viewWidget(const QString &pFileName,
                                 const bool &pCreate = true) PURE;
    virtual void removeViewWidget(const QString &pFileName) PURE;
    virtual QString viewName() const PURE;

    virtual QIcon fileTabIcon(const QString &pFileName) const PURE;

#undef PURE

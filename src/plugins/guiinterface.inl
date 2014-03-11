#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual void changeEvent(QEvent *pEvent) PURE;

    virtual void updateGui(Plugin *pViewPlugin, const QString &pFileName) PURE;

    virtual void initializeView() PURE;
    virtual void finalizeView() PURE;

    virtual bool hasViewWidget(const QString &pFileName) PURE;
    virtual QWidget * viewWidget(const QString &pFileName,
                                 const bool &pCreate = true) PURE;
    virtual void removeViewWidget(const QString &pFileName) PURE;
    virtual QString viewName() const PURE;

    virtual QIcon fileTabIcon(const QString &pFileName) const PURE;

    virtual bool saveFile(const QString &pOldFileName,
                          const QString &pNewFileName) PURE;

    virtual void fileOpened(const QString &pFileName) PURE;
    virtual void filePermissionsChanged(const QString &pFileName) PURE;
    virtual void fileModified(const QString &pFileName,
                              const bool &pModified) PURE;
    virtual void fileReloaded(const QString &pFileName) PURE;
    virtual void fileRenamed(const QString &pOldFileName,
                             const QString &pNewFileName) PURE;
    virtual void fileClosed(const QString &pFileName) PURE;

    virtual bool canClose() PURE;

#undef PURE

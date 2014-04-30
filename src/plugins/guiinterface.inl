#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual void updateGui(Plugin *pViewPlugin, const QString &pFileName) PURE;

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

#undef PURE

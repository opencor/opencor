#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual bool saveFile(const QString &pOldFileName,
                          const QString &pNewFileName) PURE;

    virtual void fileOpened(const QString &pFileName) PURE;
    virtual void filePermissionsChanged(const QString &pFileName) PURE;
    virtual void fileModified(const QString &pFileName) PURE;
    virtual void fileReloaded(const QString &pFileName) PURE;
    virtual void fileRenamed(const QString &pOldFileName,
                             const QString &pNewFileName) PURE;
    virtual void fileSaved(const QString &pFileName) PURE;
    virtual void fileClosed(const QString &pFileName) PURE;

#undef PURE

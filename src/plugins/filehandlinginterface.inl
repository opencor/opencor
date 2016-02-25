#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual bool isIndirectRemoteFile(const QString &pFileName) PURE;
    // Note: this method is called by the central widget to determine whether it
    //       should be showing its busy widget before calling viewWidget() for
    //       a given file name. Indeed, the central widget will automatically
    //       show its busy widget if a file is a remote one, but in the case of
    //       a local SED-ML file or a local COMBINE archive, it cannot tell
    //       whether it should show its busy widget since it depends on the
    //       referenced model, i.e. whether it references a local model or a
    //       remote one. So, since the central widget has no knowledge of file
    //       types, we need to ask the plugin to tell us whether a given file is
    //       an indirect remote file...

    virtual bool saveFile(const QString &pOldFileName,
                          const QString &pNewFileName,
                          bool &pNeedFeedback) PURE;

    virtual void fileOpened(const QString &pFileName) PURE;
    virtual void filePermissionsChanged(const QString &pFileName) PURE;
    virtual void fileModified(const QString &pFileName) PURE;
    virtual void fileReloaded(const QString &pFileName) PURE;
    virtual void fileRenamed(const QString &pOldFileName,
                             const QString &pNewFileName) PURE;
    virtual void fileSaved(const QString &pFileName) PURE;
    virtual void fileClosed(const QString &pFileName) PURE;

#undef PURE

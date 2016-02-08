#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual Mode viewMode() const PURE;
    virtual QStringList viewMimeTypes() const PURE;
    virtual QString viewDefaultFileExtension() const PURE;

    virtual bool showBusyWidget(const QString &pFileName) PURE;
    // Note: this method is called by the central widget to determine whether it
    //       should be showing its busy widget before calling viewWidget() for
    //       the given file name. The central widget will automatically show its
    //       busy widget if the file is a remote one, but in the case of a local
    //       SED-ML file or a local COMBINE archive, it cannot tell whether it
    //       should show its busy widget since it depends on the referenced
    //       model, i.e. whether it references a local model or a remote one.
    //       So, since the central widget has no knowledge of file types, we
    //       need to ask the plugin to tell us what to do...

    virtual bool hasViewWidget(const QString &pFileName) PURE;
    virtual QWidget * viewWidget(const QString &pFileName) PURE;
    virtual void removeViewWidget(const QString &pFileName) PURE;
    virtual QString viewName() const PURE;

    virtual QIcon fileTabIcon(const QString &pFileName) const PURE;

#undef PURE

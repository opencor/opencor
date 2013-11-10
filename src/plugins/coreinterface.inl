#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual void initialize() PURE;
    virtual void finalize() PURE;

    virtual void initialized(const Plugins &pLoadedPlugins) PURE;

    virtual void loadSettings(QSettings *pSettings) PURE;
    virtual void saveSettings(QSettings *pSettings) const PURE;

    virtual void settingsLoaded(const Plugins &pLoadedPlugins) PURE;

    virtual void handleArguments(const QStringList &pArguments) PURE;
    virtual void handleAction(const QUrl &pUrl) PURE;

#undef PURE

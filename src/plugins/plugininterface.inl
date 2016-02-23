#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual void initializePlugin() PURE;
    virtual void finalizePlugin() PURE;

    virtual void pluginsInitialized(const Plugins &pLoadedPlugins) PURE;

    virtual void loadSettings(QSettings *pSettings) PURE;
    virtual void saveSettings(QSettings *pSettings) const PURE;

    virtual void handleUrl(const QUrl &pUrl) PURE;

#undef PURE

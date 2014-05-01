#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual void initializePlugin(QMainWindow *pMainWindow) PURE;
    virtual void finalizePlugin() PURE;

    virtual void pluginInitialized(const Plugins &pLoadedPlugins) PURE;

    virtual void loadSettings(QSettings *pSettings) PURE;
    virtual void saveSettings(QSettings *pSettings) const PURE;

    virtual void handleAction(const QUrl &pUrl) PURE;

#undef PURE

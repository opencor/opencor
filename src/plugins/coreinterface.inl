#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual void initialize() PURE;
    virtual void finalize() PURE;

    virtual void initializationsDone(const Plugins &pLoadedPlugins) PURE;

    virtual void loadSettings(QSettings *pSettings) PURE;
    virtual void saveSettings(QSettings *pSettings) const PURE;

    virtual void loadingOfSettingsDone(const Plugins &pLoadedPlugins) PURE;

    virtual void handleArguments(const QStringList &pArguments) PURE;
    virtual void handleAction(const QUrl &pUrl) PURE;

    virtual void runCliCommand(const QString &pCommand,
                              const QStringList &pArguments, int *pRes) PURE;

#undef PURE

#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual QWidget * centralWidget() PURE;

    virtual void settingsLoaded(const Plugins &pLoadedPlugins) PURE;

    virtual void handleArguments(const QStringList &pArguments) PURE;

    virtual bool canClose() PURE;

#undef PURE

#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual void updateGui(Plugin *pViewPlugin, const QString &pFileName) PURE;

#undef PURE

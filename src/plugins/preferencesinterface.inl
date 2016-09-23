#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual Preferences::PreferencesWidget * preferencesWidget(QSettings *pSettings) PURE;

#undef PURE

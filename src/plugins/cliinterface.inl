#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual int execute(const QString &pCommand,
                        const QStringList &pArguments) PURE;

#undef PURE

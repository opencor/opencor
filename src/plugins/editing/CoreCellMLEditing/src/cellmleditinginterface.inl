#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual void cellmlValidation(const QString &pFileName) const PURE;

#undef PURE

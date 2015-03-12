#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual bool validCellml(const QString &pFileName) const PURE;

#undef PURE

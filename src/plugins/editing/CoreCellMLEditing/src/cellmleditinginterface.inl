#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual void reformat(const QString &pFileName) const PURE;

    virtual bool validCellml(const QString &pFileName) const PURE;

#undef PURE

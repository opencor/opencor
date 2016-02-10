#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual Solver::Solver * solverInstance() const PURE;

    virtual QString id(const QString &pKisaoId) const PURE;
    virtual QString kisaoId(const QString &pId) const PURE;

    virtual Solver::Type solverType() const PURE;
    virtual QString solverName() const PURE;
    virtual Solver::Properties solverProperties() const PURE;

    virtual QMap<QString, bool> solverPropertiesVisibility(const QMap<QString, QString> &pSolverPropertiesValues) const PURE;

#undef PURE

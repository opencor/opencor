#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual void * solverInstance() const PURE;

    virtual Solver::Type solverType() const PURE;
    virtual QString solverName() const PURE;
    virtual Solver::Properties solverProperties() const PURE;

#undef PURE

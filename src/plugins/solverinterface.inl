#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual void * instance() const PURE;

    virtual Solver::Type type() const PURE;
    virtual QString name() const PURE;
    virtual Solver::Properties properties() const PURE;

#undef PURE

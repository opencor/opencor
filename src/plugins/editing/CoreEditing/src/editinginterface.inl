#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual QScintillaSupport::QScintillaWidget * editor() const PURE;

#undef PURE

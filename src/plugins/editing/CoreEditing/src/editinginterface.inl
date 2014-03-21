#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual Editor::EditorWidget * editor(const QString &pFileName) const PURE;

#undef PURE

#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual Qt::DockWidgetArea windowDefaultDockArea() const PURE;
    virtual QAction * windowAction() const PURE;
    virtual QDockWidget * windowWidget() const PURE;

#undef PURE

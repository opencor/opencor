#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual EditorWidget::EditorWidget * editorWidget(const QString &pFileName) const PURE;

    virtual bool isEditorWidgetUseable(const QString &pFileName) const PURE;
    virtual bool isEditorWidgetContentsModified(const QString &pFileName) const PURE;

#undef PURE

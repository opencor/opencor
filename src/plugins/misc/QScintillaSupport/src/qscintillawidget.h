//==============================================================================
// QScintillaWidget class
//==============================================================================

#ifndef QSCINTILLAWIDGET_H
#define QSCINTILLAWIDGET_H

//==============================================================================

#include "qscintillasupportglobal.h"

//==============================================================================

#include "Qsci/qsciscintilla.h"

//==============================================================================

namespace OpenCOR {
namespace QScintillaSupport {

//==============================================================================

class QSCINTILLASUPPORT_EXPORT QScintillaWidget : public QsciScintilla
{
public:
    explicit QScintillaWidget(const QString &pContents, const bool &pReadOnly,
                              QsciLexer *pLexer, QWidget *pParent = 0);
    explicit QScintillaWidget(const QString &pContents, const bool &pReadOnly,
                              QWidget *pParent = 0);
    explicit QScintillaWidget(QsciLexer *pLexer, QWidget *pParent = 0);
    explicit QScintillaWidget(QWidget *pParent = 0);

    void setContents(const QString &pContents);

protected:
    virtual void dragEnterEvent(QDragEnterEvent *pEvent);

private:
    void constructor(const QString &pContents = QString(),
                     const bool &pReadOnly = false,
                     QsciLexer *pLexer = 0);
};

//==============================================================================

}   // namespace QScintillaSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

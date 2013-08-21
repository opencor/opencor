//==============================================================================
// Viewer widget
//==============================================================================

#ifndef VIEWERWIDGET_H
#define VIEWERWIDGET_H

//==============================================================================

#include "viewerglobal.h"
#include "widget.h"

//==============================================================================

#include "qwt_mml_document.h"

//==============================================================================

namespace OpenCOR {
namespace Viewer {

//==============================================================================

class VIEWER_EXPORT ViewerWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit ViewerWidget(QWidget *pParent = 0);

    QString content() const;
    void setContent(const QString &pContent);

    bool optimiseFontSize() const;
    void setOptimiseFontSize(const bool &pOptimiseFontSize);

protected:
    virtual void paintEvent(QPaintEvent *pEvent);

    virtual QSize minimumSizeHint() const;
    virtual QSize sizeHint() const;

private:
    QwtMathMLDocument mMathmlDocument;

    double mOneOverMathmlDocumentWidth;
    double mOneOverMathmlDocumentHeight;

    QString mContent;
    bool mOptimiseFontSize;
};

//==============================================================================

}   // namespace Viewer
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

//==============================================================================
// Viewer widget
//==============================================================================

#ifndef VIEWERWIDGET_H
#define VIEWERWIDGET_H

//==============================================================================

#include "commonwidget.h"
#include "viewerglobal.h"

//==============================================================================

#include "qwt_mml_document.h"
#include "qwt_text_label.h"

//==============================================================================

namespace OpenCOR {
namespace Viewer {

//==============================================================================

class VIEWER_EXPORT ViewerWidget : public QwtTextLabel,
                                   public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit ViewerWidget(QWidget *pParent = 0);

    QString content() const;
    void setContent(const QString &pContent);

    bool optimiseFontSize() const;
    void setOptimiseFontSize(const bool &pOptimiseFontSize);

protected:
    virtual bool event(QEvent *pEvent);
    virtual void paintEvent(QPaintEvent *pEvent);

    virtual QSize sizeHint() const;

private:
    QwtMathMLDocument mMathmlDocument;

    double mOneOverMathmlDocumentWidth;
    double mOneOverMathmlDocumentHeight;

    int mPaintEventLevel;

    bool mOptimiseFontSize;

    void checkOptimalFontSize();
};

//==============================================================================

}   // namespace Viewer
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

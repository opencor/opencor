//==============================================================================
// Viewer widget
//==============================================================================

#ifndef VIEWERWIDGET_H
#define VIEWERWIDGET_H

//==============================================================================

#include "widget.h"
#include "viewerglobal.h"

//==============================================================================

#include <QtMmlWidget>

//==============================================================================

namespace OpenCOR {
namespace Viewer {

//==============================================================================

class VIEWER_EXPORT ViewerWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit ViewerWidget(QWidget *pParent = 0);

    bool setContent(const QString &pContent, QString *pErrorMsg = 0,
                    int *pErrorLine = 0, int *pErrorColumn = 0);

protected:
    virtual QSize sizeHint() const;

    virtual void paintEvent(QPaintEvent *pEvent);

private:
    QtMmlDocument mMmlDocument;

    double mOneOverMmlDocumentWidth;
    double mOneOverMmlDocumentHeight;

    QString mContent;
};

//==============================================================================

}   // namespace Viewer
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

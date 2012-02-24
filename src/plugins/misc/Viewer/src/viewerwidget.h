//==============================================================================
// Viewer widget
//==============================================================================

#ifndef VIEWERWIDGET_H
#define VIEWERWIDGET_H

//==============================================================================

#include "commonwidget.h"

//==============================================================================

#include <QtMmlWidget>

//==============================================================================

namespace OpenCOR {
namespace Viewer {

//==============================================================================

class ViewerWidget : public QtMmlWidget, public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit ViewerWidget(const QString &pName, QWidget *pParent = 0);
    ~ViewerWidget();

    bool setContent(const QString &pContent, QString *pErrorMsg = 0,
                    int *pErrorLine = 0, int *pErrorColumn = 0);

protected:
    virtual QSize sizeHint() const;

    virtual void paintEvent(QPaintEvent *pEvent);
    virtual void resizeEvent(QResizeEvent *pEvent);

private:
    QtMmlWidget *mTestViewerWidget;
};

//==============================================================================

}   // namespace Viewer
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

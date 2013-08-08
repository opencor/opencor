//==============================================================================
// Viewer widget
//==============================================================================

#ifndef VIEWERWIDGET_H
#define VIEWERWIDGET_H

//==============================================================================

#include "commonwidget.h"
#include "viewerglobal.h"

//==============================================================================

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

    void setContent(const QString &pContent);

protected:
    virtual QSize sizeHint() const;

private:
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

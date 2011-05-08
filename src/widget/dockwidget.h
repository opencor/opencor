#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include "commonwidget.h"

#include <QDockWidget>

namespace OpenCOR {

class DockWidget : public QDockWidget, public CommonWidget
{
    Q_OBJECT

public:
    explicit DockWidget(QWidget *pParent);
};

}

#endif

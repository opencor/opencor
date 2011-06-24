#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include "commonwidget.h"
#include "coreglobal.h"

#include <QDockWidget>

namespace OpenCOR {

class CORE_EXPORT DockWidget : public QDockWidget, public CommonWidget
{
    Q_OBJECT

public:
    explicit DockWidget(QWidget *pParent);
};

}

#endif

#ifndef DOCKTOOLBAR_H
#define DOCKTOOLBAR_H

#include "coreglobal.h"

#include <QToolBar>

namespace OpenCOR {
namespace Core {

class CORE_EXPORT DockToolBar : public QToolBar
{
    Q_OBJECT

public:
    explicit DockToolBar(QWidget *pParent = 0);
};

} }

#endif

#ifndef PMREXPLORERWIDGET_H
#define PMREXPLORERWIDGET_H

#include "commonwidget.h"

#include <QWidget>

class PmrExplorerWidget : public QWidget, public CommonWidget
{
    Q_OBJECT

public:
    virtual void retranslateUi();

    virtual void defaultSettings();
};

#endif

#ifndef FILEORGANISERWIDGET_H
#define FILEORGANISERWIDGET_H

#include "commonwidget.h"

#include <QWidget>

class FileOrganiserWidget : public QWidget, public CommonWidget
{
    Q_OBJECT

public:
    virtual void retranslateUi();

    virtual void defaultSettings();
};

#endif

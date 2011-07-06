#ifndef CELLMLMODELREPOSITORYWIDGET_H
#define CELLMLMODELREPOSITORYWIDGET_H

#include "commonwidget.h"

#include <QWebView>

namespace OpenCOR {
namespace CellMLModelRepository {

class CellmlModelRepositoryWidget : public QWebView, public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit CellmlModelRepositoryWidget(const QString &pName,
                                         QWidget *pParent);

protected:
    virtual QSize sizeHint() const;

    virtual void paintEvent(QPaintEvent *pEvent);
};

} }

#endif

//==============================================================================
// CellML annotation view widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWWIDGET_H
#define CELLMLANNOTATIONVIEWWIDGET_H

//==============================================================================

#include "cellmlfile.h"
#include "commonwidget.h"

//==============================================================================

#include <QSplitter>

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewWidget;
}

//==============================================================================

class QLabel;

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewListsWidget;
class CellmlAnnotationViewDetailsWidget;
class CellMLAnnotationViewPlugin;

//==============================================================================

class CellmlAnnotationViewWidget : public QSplitter, public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewWidget(QWidget *pParent,
                                        CellMLAnnotationViewPlugin *pPluginParent,
                                        const QString &pFileName);
    ~CellmlAnnotationViewWidget();

    virtual void retranslateUi();

    CellMLAnnotationViewPlugin * pluginParent() const;

    CellMLSupport::CellmlFile * cellmlFile() const;

    CellmlAnnotationViewListsWidget * listsWidget() const;
    CellmlAnnotationViewDetailsWidget * detailsWidget() const;

    static QLabel * newLabel(QWidget *pParent, const QString &pText,
                             const bool &pBold = false,
                             const double &pFontPercentage = 1.0,
                             const Qt::Alignment &pAlignment = Qt::AlignLeft|Qt::AlignVCenter);
    static QLabel * newLabelLink(QWidget *pParent, const QString &pText,
                                 const bool &pBold = false,
                                 const double &pFontPercentage = 1.0,
                                 const Qt::Alignment &pAlignment = Qt::AlignLeft|Qt::AlignVCenter);

private:
    Ui::CellmlAnnotationViewWidget *mGui;

    CellMLAnnotationViewPlugin *mPluginParent;

    CellMLSupport::CellmlFile *mCellmlFile;

    CellmlAnnotationViewListsWidget *mListsWidget;
    CellmlAnnotationViewDetailsWidget *mDetailsWidget;

Q_SIGNALS:
    void splitterMoved(const QList<int> &pSizes);

public Q_SLOTS:
    void updateSizes(const QList<int> &pSizes);

private Q_SLOTS:
    void emitSplitterMoved();
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

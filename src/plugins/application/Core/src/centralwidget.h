//==============================================================================
// Central widget
//==============================================================================

#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

//==============================================================================

#include "commonwidget.h"
#include "guiinterface.h"

//==============================================================================

#include <QMap>
#include <QTabBar>
#include <QWidget>

//==============================================================================

namespace Ui {
    class CentralWidget;
}

//==============================================================================

class QLabel;
class QStackedWidget;

//==============================================================================

namespace OpenCOR {

//==============================================================================

class Plugin;

//==============================================================================

namespace Core {

//==============================================================================

class CentralWidget : public QWidget, public CommonWidget
{
    Q_OBJECT

public:
    explicit CentralWidget(QWidget *pParent = 0);
    ~CentralWidget();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    bool openFile(const QString &pFileName);
    bool activateFile(const QString &pFileName);

    int nbOfFilesOpened() const;

    QString activeFileName() const;

    bool isModeEnabled(const GuiViewSettings::Mode &pMode) const;

    void addView(Plugin *pPlugin, GuiViewSettings *pSettings);

protected:
    virtual void dragEnterEvent(QDragEnterEvent *pEvent);
    virtual void dragMoveEvent(QDragMoveEvent *pEvent);
    virtual void dropEvent(QDropEvent *pEvent);
    virtual void paintEvent(QPaintEvent *pEvent);

private:
    Ui::CentralWidget *mUi;

    bool mShuttingDown;

    QTabBar *mModes;

    QTabBar *mFiles;
    QStackedWidget *mContents;

    QWidget *mNoView;
    QLabel *mNoViewMsg;

    QTabBar *mEditingViews;
    QTabBar *mAnalysisViews;

    QMap<GuiViewSettings::Mode, bool> mModeEnabled;

    QMap<int, QString> mEditingViewPluginNames;
    QMap<int, QString> mAnalysisViewPluginNames;

    QMap<int, GuiInterface *> mEditingViewInterfaces;
    GuiInterface *mSimulationViewInterface;
    QMap<int, GuiInterface *> mAnalysisViewInterfaces;

    QMap<int, GuiViewSettings *> mEditingViewSettings;
    QMap<int, GuiViewSettings *> mAnalysisViewSettings;

    QPixmap mLogo;

    int mLogoWidth;
    int mLogoHeight;

    int modeTabIndex(const GuiViewSettings::Mode &pMode) const;

    void addMode(const GuiViewSettings::Mode &pMode);

    QTabBar * newTabBar(const QTabBar::Shape &pShape,
                        const bool &pMovable = false,
                        const bool &pTabsClosable = false);

    void updateNoViewMsg();

Q_SIGNALS:
    void fileOpened(const QString &pFileName);
    void fileClosed(const QString &pFileName);
    void fileSelected(const QString &pFileName);

public Q_SLOTS:
    void openFiles(const QStringList &pFileNames);
    void selectPreviousFile();
    void selectNextFile();
    bool closeFile(const int &pIndex = -1);
    void closeAllFiles();
    void fileSelected(const int &pIndex);
    void updateGui();
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

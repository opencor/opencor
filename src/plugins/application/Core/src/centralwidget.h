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

class LogoWidget;

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

private:
    enum Status {
        Starting,
        Running,
        Stopping
    };

    Ui::CentralWidget *mUi;

    Status mStatus;

    QTabBar *mModes;

    QTabBar *mFiles;
    QStackedWidget *mContents;

    LogoWidget *mLogoView;

    QWidget *mNoView;
    QLabel *mNoViewMsg;

    QTabBar *mEditingViews;
    QTabBar *mSimulationViews;
    QTabBar *mAnalysisViews;

    QMap<GuiViewSettings::Mode, bool> mModeEnabled;

    QMap<int, QString> mEditingViewPluginNames;
    QMap<int, QString> mSimulationViewPluginNames;
    QMap<int, QString> mAnalysisViewPluginNames;

    QMap<int, GuiInterface *> mEditingViewInterfaces;
    QMap<int, GuiInterface *> mSimulationViewInterfaces;
    QMap<int, GuiInterface *> mAnalysisViewInterfaces;

    QMap<int, GuiViewSettings *> mEditingViewSettings;
    QMap<int, GuiViewSettings *> mSimulationViewSettings;
    QMap<int, GuiViewSettings *> mAnalysisViewSettings;

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
    void previousFile();
    void nextFile();
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

/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// GUI interface
//==============================================================================

#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

//==============================================================================

#include <Qt>

//==============================================================================

#include <QIcon>
#include <QList>
#include <QKeySequence>
#include <QObject>
#include <QString>

//==============================================================================

class QAction;
class QDockWidget;
class QMainWindow;
class QMenu;
class QSettings;
class QTabBar;
class QToolBar;
class QWidget;

//==============================================================================

namespace OpenCOR {

//==============================================================================

class Plugin;

//==============================================================================

class GuiMenuSettings
{
public:
    enum Type {
        File,
        View
    };

    explicit GuiMenuSettings(const Type &pType, QMenu *pMenu);
    explicit GuiMenuSettings(const Type &pType, QAction *pAction, QMenu *pMenu);

    Type type() const;
    QAction * action() const;
    QMenu * menu() const;

private:
    Type mType;
    QAction *mAction;
    QMenu *mMenu;
};

//==============================================================================

class GuiMenuActionSettings
{
public:
    enum Type {
        File,
        FileNew,
        Tools
    };

    explicit GuiMenuActionSettings(const Type &pType, QAction *pAction);

    Type type() const;
    QAction * action() const;

private:
    Type mType;
    QAction *mAction;
};

//==============================================================================

static const auto ModeUnknown    = QStringLiteral("Unknown");
static const auto ModeEditing    = QStringLiteral("Editing");
static const auto ModeSimulation = QStringLiteral("Simulation");
static const auto ModeAnalysis   = QStringLiteral("Analysis");

//==============================================================================

class GuiViewSettings
{
public:
    enum Mode {
        Unknown,
        Editing,
        Simulation,
        Analysis
    };

    explicit GuiViewSettings(const Mode &pMode, const QStringList &pMimeTypes);

    Mode mode() const;
    QStringList mimeTypes() const;

    static QString modeAsString(const Mode &pMode);
    static Mode modeFromString(const QString &pMode);

private:
    Mode mMode;
    QStringList mMimeTypes;
};

//==============================================================================

class GuiWindowSettings
{
public:
    enum Type {
        Organisation,
        Editing,
        Help
    };

    explicit GuiWindowSettings(const Qt::DockWidgetArea &pDefaultDockArea,
                               QWidget *pWindow, const Type &pType,
                               QAction *pAction);

    Qt::DockWidgetArea defaultDockArea() const;
    QWidget * window() const;
    Type type() const;
    QAction * action() const;

private:
    Qt::DockWidgetArea mDefaultDockArea;
    QWidget *mWindow;
    Type mType;
    QAction *mAction;
};

//==============================================================================

class GuiSettings
{
public:
    explicit GuiSettings();
    ~GuiSettings();

    void addMenu(const GuiMenuSettings::Type &pType, QMenu *pMenu);
    void addMenu(const GuiMenuSettings::Type &pType, QAction *pAction,
                 QMenu *pMenu);
    void addMenuAction(const GuiMenuActionSettings::Type &pType,
                       QAction *pAction = 0);
    void setCentralWidget(QWidget *pCentralWidget);
    void addWindow(const Qt::DockWidgetArea &pDefaultDockArea,
                   QWidget *pWindow, const GuiWindowSettings::Type &pType,
                   QAction *pAction);
    void setView(const GuiViewSettings::Mode &pMode,
                 const QStringList &pMimeTypes);

    QList<GuiMenuSettings *> menus() const;
    QList<GuiMenuActionSettings *> menuActions() const;
    QWidget * centralWidget() const;
    QList<GuiWindowSettings *> windows() const;
    GuiViewSettings * view() const;

private:
    QList<GuiMenuSettings *> mMenus;
    QList<GuiMenuActionSettings *> mMenuActions;
    QWidget *mCentralWidget;
    GuiViewSettings *mView;
    QList<GuiWindowSettings *> mWindows;
};

//==============================================================================

class GuiInterface
{
    friend class MainWindow;

public:
    explicit GuiInterface();
    ~GuiInterface();

#define INTERFACE_DEFINITION
    #include "guiinterface.inl"
#undef INTERFACE_DEFINITION

    GuiSettings * guiSettings() const;

    static QMenu * newMenu(const QString &pName, QWidget *pParent = 0);
    static QMenu * newMenu(const QIcon &pIcon, QWidget *pParent = 0);

    static QAction * newAction(const bool &pCheckable, const QIcon &pIcon,
                               const QKeySequence &pKeySequence,
                               QWidget *pParent);
    static QAction * newAction(const bool &pCheckable,
                               const QKeySequence &pKeySequence,
                               QWidget *pParent = 0);
    static QAction * newAction(const bool &pCheckable, QWidget *pParent = 0);
    static QAction * newAction(const QIcon &pIcon,
                               const QKeySequence &pKeySequence,
                               QWidget *pParent = 0);
    static QAction * newAction(const QIcon &pIcon, QWidget *pParent = 0);
    static QAction * newAction(const QKeySequence &pKeySequence,
                               QWidget *pParent = 0);
    static QAction * newAction(const QKeySequence::StandardKey &pStandardKey,
                               QWidget *pParent = 0);

    static void retranslateMenu(QMenu *pMenu, const QString &pTitle);
    static void retranslateAction(QAction *pAction, const QString &pText,
                                  const QString &pStatusTip = QString());

protected:
    QMainWindow *mMainWindow;

    GuiSettings *mGuiSettings;

private:
    void setMainWindow(QMainWindow *pMainWindow);
};

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

Q_DECLARE_INTERFACE(OpenCOR::GuiInterface, "OpenCOR::GuiInterface")

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

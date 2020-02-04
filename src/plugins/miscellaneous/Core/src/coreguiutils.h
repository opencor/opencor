/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// Core GUI utilities
//==============================================================================

#pragma once

//==============================================================================

#include "coreglobal.h"
#include "file.h"

//==============================================================================

#include <QColor>
#include <QIcon>
#include <QKeySequence>
#include <QMessageBox>
#include <QSettings>
#include <QStyledItemDelegate>
#include <QString>

//==============================================================================

class QAction;
class QFrame;
class QLabel;
class QMainWindow;
class QMenu;
class QWidget;

//==============================================================================

namespace OpenCOR {

//==============================================================================

class Plugin;

//==============================================================================

namespace Core {

//==============================================================================

class CentralWidget;

//==============================================================================

#include "guiutils.h.inl"

//==============================================================================
// Note: both guiutils.h and coreguiutils.h must specifically define
//       Dialog. To have it in guiutils.h.inl is NOT good enough since the MOC
//       won't pick it up...

class CORE_EXPORT Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *pParent);

    bool hasPositionAndSize();

protected:
    QSettings mSettings;

    void resizeEvent(QResizeEvent *pEvent) override;

public slots:
    int exec() override;
};

//==============================================================================
// Note: both guiutils.h and coreguiutils.h must specifically define
//       StyledItemDelegate. To have it in guiutils.h.inl is NOT good enough
//       since the MOC won't pick it up...

class CORE_EXPORT StyledItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit StyledItemDelegate(QObject *pParent);

    QSize sizeHint(const QStyleOptionViewItem &pOption,
                   const QModelIndex &pIndex) const override;
};

//==============================================================================

CentralWidget CORE_EXPORT * centralWidget();

void CORE_EXPORT showCentralBusyWidget();
void CORE_EXPORT showCentralProgressBusyWidget();
void CORE_EXPORT setCentralBusyWidgetProgress(double pProgress);
void CORE_EXPORT hideCentralBusyWidget();

Plugin CORE_EXPORT * currentViewPlugin();

QString CORE_EXPORT currentFileName();

QString CORE_EXPORT getOpenFileName(const QString &pCaption,
                                    const QStringList &pFilters = {},
                                    QString *pSelectedFilter = nullptr);
QStringList CORE_EXPORT getOpenFileNames(const QString &pCaption,
                                         const QStringList &pFilters = {},
                                         QString *pSelectedFilter = nullptr);
QString CORE_EXPORT getSaveFileName(const QString &pCaption,
                                    const QString &pFileName,
                                    const QStringList &pFilters = {},
                                    QString *pSelectedFilter = nullptr);
QString CORE_EXPORT getSaveFileName(const QString &pCaption,
                                    const QStringList &pFilters = {},
                                    QString *pSelectedFilter = nullptr);

QString CORE_EXPORT getDirectory(const QString &pCaption,
                                 const QString &pDirName = {},
                                 bool pEmptyDir = false);
QString CORE_EXPORT getEmptyDirectory(const QString &pCaption,
                                      const QString &pDirName = {});

QString CORE_EXPORT guiOpenFile(const QString &pFileName,
                                File::Type pType = File::Type::Local,
                                const QString &pUrl = {},
                                bool pShowWarning = true);
QString CORE_EXPORT guiOpenRemoteFile(const QString &pUrl,
                                      bool pShowWarning = true);

bool CORE_EXPORT guiCloseFile(const QString &pFileName);

void CORE_EXPORT setFocusTo(QWidget *pWidget);

QMenu CORE_EXPORT * newMenu(const QString &pName, QWidget *pParent);
QMenu CORE_EXPORT * newMenu(const QIcon &pIcon, QWidget *pParent);

QAction CORE_EXPORT * newAction(bool pCheckable, const QIcon &pIcon,
                                const QKeySequence &pKeySequence,
                                QWidget *pParent);
QAction CORE_EXPORT * newAction(bool pCheckable, const QIcon &pIcon,
                                QWidget *pParent);
QAction CORE_EXPORT * newAction(bool pCheckable,
                                const QKeySequence &pKeySequence,
                                QWidget *pParent);
QAction CORE_EXPORT * newAction(bool pCheckable, QWidget *pParent);
QAction CORE_EXPORT * newAction(const QIcon &pIcon,
                                const QList<QKeySequence> &pKeySequences,
                                QWidget *pParent);
QAction CORE_EXPORT * newAction(const QIcon &pIcon,
                                const QKeySequence &pKeySequence,
                                QWidget *pParent);
QAction CORE_EXPORT * newAction(const QIcon &pIcon, QWidget *pParent);
QAction CORE_EXPORT * newAction(const QKeySequence &pKeySequence,
                                QWidget *pParent);
QAction CORE_EXPORT * newAction(const QKeySequence::StandardKey &pStandardKey,
                                QWidget *pParent);
QAction CORE_EXPORT * newAction(QWidget *pParent);

QAction CORE_EXPORT * newSeparator(QWidget *pParent);

QFrame CORE_EXPORT * newLineWidget(bool pHorizontal, QWidget *pParent);
QFrame CORE_EXPORT * newLineWidget(QWidget *pParent);

QString CORE_EXPORT iconDataUri(const QIcon &pIcon, int pWidth, int pHeight,
                                QIcon::Mode pMode = QIcon::Normal);
QString CORE_EXPORT iconDataUri(const QString &pIcon, int pWidth, int pHeight,
                                QIcon::Mode pMode = QIcon::Normal);

QIcon CORE_EXPORT standardIcon(QStyle::StandardPixmap pStandardIcon,
                               const QStyleOption *pOption = nullptr,
                               const QWidget *pWidget = nullptr);

QIcon CORE_EXPORT tintedIcon(const QIcon &pIcon, const QColor &pColor);
QIcon CORE_EXPORT tintedIcon(const QString &pIcon, const QColor &pColor);

QIcon CORE_EXPORT overlayedIcon(const QIcon &pBaseIcon,
                                const QIcon &pOverlayIcon,
                                int pBaseWidth, int pBaseHeight,
                                int pOverlayLeft, int pOverlayTop,
                                int pOverlayWidth, int pOverlayHeight);
QIcon CORE_EXPORT overlayedIcon(const QString &pBaseIcon,
                                const QIcon &pOverlayIcon,
                                int pBaseWidth, int pBaseHeight,
                                int pOverlayLeft, int pOverlayTop,
                                int pOverlayWidth, int pOverlayHeight);
QIcon CORE_EXPORT overlayedIcon(const QIcon &pBaseIcon,
                                const QString &pOverlayIcon,
                                int pBaseWidth, int pBaseHeight,
                                int pOverlayLeft, int pOverlayTop,
                                int pOverlayWidth, int pOverlayHeight);
QIcon CORE_EXPORT overlayedIcon(const QString &pBaseIcon,
                                const QString &pOverlayIcon,
                                int pBaseWidth, int pBaseHeight,
                                int pOverlayLeft, int pOverlayTop,
                                int pOverlayWidth, int pOverlayHeight);

QIcon CORE_EXPORT scaledIcon(const QIcon &pIcon, int pWidth, int pHeight,
                             Qt::AspectRatioMode pAspectMode = Qt::IgnoreAspectRatio,
                             Qt::TransformationMode pMode = Qt::FastTransformation);
QIcon CORE_EXPORT scaledIcon(const QString &pIcon, int pWidth, int pHeight,
                             Qt::AspectRatioMode pAspectMode = Qt::IgnoreAspectRatio,
                             Qt::TransformationMode pMode = Qt::FastTransformation);

void CORE_EXPORT showEnableWidget(QWidget *pWidget, bool pVisible,
                                  bool pEnabled = true);

QColor CORE_EXPORT lockedColor(const QColor &pColor);

bool CORE_EXPORT opencorActive();

//==============================================================================

} // namespace Core
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

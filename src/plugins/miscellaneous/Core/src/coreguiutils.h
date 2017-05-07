/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Core GUI utilities
//==============================================================================

#pragma once

//==============================================================================

#include "coreglobal.h"
#include "filetypeinterface.h"

//==============================================================================

#include <QColor>
#include <QIcon>
#include <QKeySequence>
#include <QMessageBox>
#include <QString>

//==============================================================================

class QAction;
class QFrame;
class QLabel;
class QMainWindow;
class QMenu;
class QWidget;
class QSettings;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CentralWidget;

//==============================================================================

#include "coreguiutils.h.inl"

//==============================================================================
// Note: both guiutils.h and coreguiutils.h must specifically define
//       Dialog. To have it in guiutils.h.inl is NOT good enough since the MOC
//       won't pick it up...

class CORE_EXPORT Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QSettings *pSettings, QWidget *pParent);
    explicit Dialog(QWidget *pParent);

    int exec(QSettings *pSettings);

    bool hasPositionAndSize();

protected:
    QSettings *mSettings;

    virtual void resizeEvent(QResizeEvent *pEvent);

public slots:
    virtual int exec();
};

//==============================================================================

CentralWidget CORE_EXPORT * centralWidget();

bool CORE_EXPORT readFileContentsFromUrlWithBusyWidget(const QString &pUrl,
                                                       QByteArray &pFileContents,
                                                       QString *pErrorMessage = 0);
bool CORE_EXPORT readFileContentsFromUrlWithBusyWidget(const QString &pUrl,
                                                       QString &pFileContents,
                                                       QString *pErrorMessage = 0);

QString CORE_EXPORT getOpenFileName(const QString &pCaption,
                                    const QStringList &pFilters = QStringList(),
                                    QString *pSelectedFilter = 0);
QStringList CORE_EXPORT getOpenFileNames(const QString &pCaption,
                                         const QStringList &pFilters = QStringList(),
                                         QString *pSelectedFilter = 0);
QString CORE_EXPORT getSaveFileName(const QString &pCaption,
                                    const QString &pFileName,
                                    const QStringList &pFilters = QStringList(),
                                    QString *pSelectedFilter = 0);

QString CORE_EXPORT getDirectory(const QString &pCaption,
                                 const QString &pDirName = QString(),
                                 const bool &pEmptyDir = false);
QString CORE_EXPORT getEmptyDirectory(const QString &pCaption,
                                      const QString &pDirName = QString());

void CORE_EXPORT setFocusTo(QWidget *pWidget);

QMenu CORE_EXPORT * newMenu(const QString &pName, QWidget *pParent);
QMenu CORE_EXPORT * newMenu(const QIcon &pIcon, QWidget *pParent);

QAction CORE_EXPORT * newAction(const bool &pCheckable, const QIcon &pIcon,
                                const QKeySequence &pKeySequence,
                                QWidget *pParent);
QAction CORE_EXPORT * newAction(const bool &pCheckable, const QIcon &pIcon,
                                QWidget *pParent);
QAction CORE_EXPORT * newAction(const bool &pCheckable,
                                const QKeySequence &pKeySequence,
                                QWidget *pParent);
QAction CORE_EXPORT * newAction(const bool &pCheckable, QWidget *pParent);
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

QFrame CORE_EXPORT * newLineWidget(const bool &pHorizontal,
                                   const QColor &pColor, QWidget *pParent);
QFrame CORE_EXPORT * newLineWidget(const bool &pHorizontal, QWidget *pParent);
QFrame CORE_EXPORT * newLineWidget(const QColor &pColor, QWidget *pParent);
QFrame CORE_EXPORT * newLineWidget(QWidget *pParent);

QString CORE_EXPORT iconDataUri(const QIcon &pIcon, const int &pWidth,
                                const int &pHeight,
                                const QIcon::Mode &pMode = QIcon::Normal);
QString CORE_EXPORT iconDataUri(const QString &pIcon, const int &pWidth,
                                const int &pHeight,
                                const QIcon::Mode &pMode = QIcon::Normal);

QIcon CORE_EXPORT tintedIcon(const QIcon &pIcon, const int &pWidth,
                             const int &pHeight, const QColor &pColor);
QIcon CORE_EXPORT tintedIcon(const QString &pIcon, const int &pWidth,
                             const int &pHeight, const QColor &pColor);

QIcon CORE_EXPORT overlayedIcon(const QIcon &pBaseIcon,
                                const QIcon &pOverlayIcon,
                                const int &pBaseWidth, const int &pBaseHeight,
                                const int &pOverlayLeft, const int &pOverlayTop,
                                const int &pOverlayWidth,
                                const int &pOverlayHeight);
QIcon CORE_EXPORT overlayedIcon(const QString &pBaseIcon,
                                const QIcon &pOverlayIcon,
                                const int &pBaseWidth, const int &pBaseHeight,
                                const int &pOverlayLeft, const int &pOverlayTop,
                                const int &pOverlayWidth,
                                const int &pOverlayHeight);
QIcon CORE_EXPORT overlayedIcon(const QIcon &pBaseIcon,
                                const QString &pOverlayIcon,
                                const int &pBaseWidth, const int &pBaseHeight,
                                const int &pOverlayLeft, const int &pOverlayTop,
                                const int &pOverlayWidth,
                                const int &pOverlayHeight);
QIcon CORE_EXPORT overlayedIcon(const QString &pBaseIcon,
                                const QString &pOverlayIcon,
                                const int &pBaseWidth, const int &pBaseHeight,
                                const int &pOverlayLeft, const int &pOverlayTop,
                                const int &pOverlayWidth,
                                const int &pOverlayHeight);

void CORE_EXPORT showEnableWidget(QWidget *pWidget, const bool &pVisible,
                                  const bool &pEnabled = true);

QColor CORE_EXPORT lockedColor(const QColor &pColor);

QStringList CORE_EXPORT filters(const FileTypeInterfaces &pFileTypeInterfaces);
QStringList CORE_EXPORT filters(const FileTypeInterfaces &pFileTypeInterfaces,
                                const QStringList &pMimeTypes);

bool CORE_EXPORT opencorActive();

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

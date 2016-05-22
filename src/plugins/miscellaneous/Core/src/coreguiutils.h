/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

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
namespace Core {

//==============================================================================

class CentralWidget;

//==============================================================================

#include "coreguiutils.h.inl"

//==============================================================================

CentralWidget CORE_EXPORT * centralWidget();

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

QString CORE_EXPORT getExistingDirectory(const QString &pCaption,
                                         const QString &pDirName = QString(),
                                         const bool &pEmptyDir = false);

void CORE_EXPORT setFocusTo(QWidget *pWidget);

QMenu CORE_EXPORT * newMenu(const QString &pName, QWidget *pParent);
QMenu CORE_EXPORT * newMenu(const QIcon &pIcon, QWidget *pParent);

QAction CORE_EXPORT * newAction(const bool &pCheckable, const QIcon &pIcon,
                                const QKeySequence &pKeySequence,
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

QFrame CORE_EXPORT * newLineWidget(const bool &pHorizontal,
                                   const QColor &pColor, QWidget *pParent);
QFrame CORE_EXPORT * newLineWidget(const bool &pHorizontal, QWidget *pParent);
QFrame CORE_EXPORT * newLineWidget(const QColor &pColor, QWidget *pParent);
QFrame CORE_EXPORT * newLineWidget(QWidget *pParent);

QString CORE_EXPORT iconDataUri(const QString &pIcon, const int &pWidth = -1,
                                const int &pHeight = -1,
                                const QIcon::Mode &pMode = QIcon::Normal);

void CORE_EXPORT showEnableWidget(QWidget *pWidget, const bool &pVisible,
                                  const bool &pEnabled = true);

QColor CORE_EXPORT lockedColor(const QColor &pColor);

QStringList CORE_EXPORT filters(const FileTypes &pFileTypes);
QStringList CORE_EXPORT filters(const FileTypes &pFileTypes,
                                const QStringList &pMimeTypes);

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

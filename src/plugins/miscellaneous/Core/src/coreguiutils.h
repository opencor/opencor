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
// Core GUI utilities
//==============================================================================

#ifndef COREGUIUTILS_H
#define COREGUIUTILS_H

//==============================================================================

#include "coreglobal.h"

//==============================================================================

#include <QColor>
#include <QIcon>
#include <QKeySequence>
#include <QString>

//==============================================================================

class QAction;
class QFrame;
class QLabel;
class QMenu;
class QWidget;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

#include "coreguiutils.h.inl"

//==============================================================================

QString CORE_EXPORT getOpenFileName(const QString &pCaption,
                                    const QString &pFilters = QString());
QStringList CORE_EXPORT getOpenFileNames(const QString &pCaption,
                                         const QString &pFilters = QString());
QString CORE_EXPORT getSaveFileName(const QString &pCaption,
                                    const QString &pFileName,
                                    const QString &pFilters = QString());

void CORE_EXPORT setFocusTo(QWidget *pWidget);

QMenu CORE_EXPORT * newMenu(const QString &pName, QWidget *pParent = 0);
QMenu CORE_EXPORT * newMenu(const QIcon &pIcon, QWidget *pParent = 0);

QAction CORE_EXPORT * newAction(const bool &pCheckable, const QIcon &pIcon,
                                const QKeySequence &pKeySequence,
                                QWidget *pParent);
QAction CORE_EXPORT * newAction(const bool &pCheckable,
                                const QKeySequence &pKeySequence,
                                QWidget *pParent = 0);
QAction CORE_EXPORT * newAction(const bool &pCheckable, QWidget *pParent = 0);
QAction CORE_EXPORT * newAction(const QIcon &pIcon,
                                const QList<QKeySequence> &pKeySequences,
                                QWidget *pParent = 0);
QAction CORE_EXPORT * newAction(const QIcon &pIcon,
                                const QKeySequence &pKeySequence,
                                QWidget *pParent = 0);
QAction CORE_EXPORT * newAction(const QIcon &pIcon, QWidget *pParent = 0);
QAction CORE_EXPORT * newAction(const QKeySequence &pKeySequence,
                                QWidget *pParent = 0);
QAction CORE_EXPORT * newAction(const QKeySequence::StandardKey &pStandardKey,
                                QWidget *pParent = 0);

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

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

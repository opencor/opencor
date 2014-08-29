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
// GUI utilities
//==============================================================================

#include "cliutils.h"
#include "commonwidget.h"
#include "coresettings.h"
#include "guiutils.h"
#include "settings.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QAction>
#include <QApplication>
#include <QColor>
#include <QDate>
#include <QFileDialog>
#include <QFileInfo>
#include <QFont>
#include <QFrame>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPalette>
#include <QSettings>
#include <QSizePolicy>
#include <QStackedWidget>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

QString allFilters(const QString &pFilters)
{
    return  QObject::tr("All Files")
           +" (*"
#ifdef Q_OS_WIN
           +".*"
#endif
           +")"
           +(pFilters.isEmpty()?QString():";;"+pFilters);
}

//==============================================================================

QString getOpenFileName(const QString &pCaption, const QString &pFilters)
{
    // Retrieve and return one open file name

    QString res = QFileDialog::getOpenFileName(qApp->activeWindow(),
                                               pCaption, activeDirectory(),
                                               allFilters(pFilters));

    if (res.size())
        // We have retrieved a file name, so keep track of the folder in which
        // it is

        setActiveDirectory(QFileInfo(res).path());

    // Return the file name

    return res;
}

//==============================================================================

QStringList getOpenFileNames(const QString &pCaption, const QString &pFilters)
{
    // Retrieve and return one or several open file names

    QStringList res = QFileDialog::getOpenFileNames(qApp->activeWindow(),
                                                    pCaption, activeDirectory(),
                                                    allFilters(pFilters));

    if (res.count())
        // We have retrieved at least one file name, so keep track of the folder
        // in which it is
        // Note #1: we use the last open file name to determine the folder that
        //          is to be remembered since on Windows 7, at least, it's
        //          possible to search for files from within the file dialog
        //          box, and the last file name should be the one we are most
        //          'interested' in...
        // Note #2: this doesn't, unfortunately, address the case where the user
        //          goes to a directory and then closes the file dialog box
        //          without selecting any open file name. There might be a way
        //          to get it to work, but that would involve using the exec()
        //          method rather than the static getOpenFilesNames() method,
        //          which would result in a non-native looking file dialog box
        //          (on Windows 7 at least), so it's not an option
        //          unfortunately...

        setActiveDirectory(QFileInfo(res[res.count()-1]).path());

    // Return the file name(s)

    return res;
}

//==============================================================================

QString getSaveFileName(const QString &pCaption, const QString &pFileName,
                        const QString &pFilters)
{
    // Retrieve and return a save file name

    QString res = QDir::toNativeSeparators(QFileDialog::getSaveFileName(qApp->activeWindow(),
                                                                        pCaption,
                                                                        pFileName.isEmpty()?
                                                                            activeDirectory():
                                                                            pFileName,
                                                                        allFilters(pFilters), 0,
                                                                        QFileDialog::DontConfirmOverwrite));

    // Make sure that we have got a save file name

    if (!res.isEmpty()) {
        // Make sure that the save file name is not the same as our given one

        if (!res.compare(pFileName)) {
            QMessageBox::warning(qApp->activeWindow(), pCaption,
                                 QObject::tr("Please choose a different file name."));

            return QString();
        }

        // Update our active directory

        QFileInfo resInfo = res;

        setActiveDirectory(resInfo.path());

        // Check whether the save file already exists

        if (resInfo.exists())
            // The save file already exists, so ask whether we want to overwrite
            // it

            if (QMessageBox::question(qApp->activeWindow(), pCaption,
                                      QObject::tr("<strong>%1</strong> already exists. Do you want to overwrite it?").arg(res),
                                      QMessageBox::Yes|QMessageBox::No,
                                      QMessageBox::Yes) == QMessageBox::No )
                // We don't want to overwrite the save file, so...

                return QString();
    }

    // Everything went fine,so return the save file name

    return res;
}

//==============================================================================

void setFocusTo(QWidget *pWidget)
{
    // Give the focus to pWidget, but then revert the focus back to whoever had
    // it before, if needed

    if (!pWidget)
        return;

    QWidget *focusedWidget = qApp->activeWindow()?
                                 qApp->activeWindow()->focusWidget():
                                 0;

    if (   !focusedWidget
        || (pWidget->parentWidget() == focusedWidget->parentWidget())) {
        // Either there is no currently focused widget or the currently focused
        // widget and the one to which we want to set the focus share the same
        // parent, so set the focus to the given widget

        pWidget->setFocus();
    }
}

//==============================================================================

QMenu * newMenu(const QString &pName, QWidget *pParent)
{
    // Create and return a menu

    QMenu *res = new QMenu(pParent);

    res->setObjectName("menu"+pName.left(1).toUpper()+pName.right(pName.size()-1));

    return res;
}

//==============================================================================

QMenu * newMenu(const QIcon &pIcon, QWidget *pParent)
{
    // Create and return a menu

    QMenu *res = new QMenu(pParent);

    res->menuAction()->setIcon(pIcon);

    return res;
}

//==============================================================================

QAction * newAction(const bool &pCheckable, const QIcon &pIcon,
                    const QKeySequence &pKeySequence, QWidget *pParent)
{
    // Create and return an action

    QAction *res = new QAction(pParent);

    res->setCheckable(pCheckable);
    res->setIcon(pIcon);
    res->setShortcut(pKeySequence);

    return res;
}

//==============================================================================

QAction * newAction(const bool &pCheckable, const QKeySequence &pKeySequence,
                    QWidget *pParent)
{
    // Create and return an action

    QAction *res = new QAction(pParent);

    res->setCheckable(pCheckable);
    res->setShortcut(pKeySequence);

    return res;
}

//==============================================================================

QAction * newAction(const bool &pCheckable, QWidget *pParent)
{
    // Create and return an action

    QAction *res = new QAction(pParent);

    res->setCheckable(pCheckable);

    return res;
}

//==============================================================================

QAction * newAction(const QIcon &pIcon,
                    const QList<QKeySequence> &pKeySequences, QWidget *pParent)
{
    // Create and return an action

    QAction *res = new QAction(pParent);

    res->setIcon(pIcon);
    res->setShortcuts(pKeySequences);

    return res;
}

//==============================================================================

QAction * newAction(const QIcon &pIcon, const QKeySequence &pKeySequence,
                    QWidget *pParent)
{
    // Create and return an action

    QAction *res = new QAction(pParent);

    res->setIcon(pIcon);
    res->setShortcut(pKeySequence);

    return res;
}

//==============================================================================

QAction * newAction(const QIcon &pIcon, QWidget *pParent)
{
    // Create and return an action

    QAction *res = new QAction(pParent);

    res->setIcon(pIcon);

    return res;
}

//==============================================================================

QAction * newAction(const QKeySequence &pKeySequence, QWidget *pParent)
{
    // Create and return an action

    QAction *res = new QAction(pParent);

    res->setShortcut(pKeySequence);

    return res;
}

//==============================================================================

QAction * newAction(const QKeySequence::StandardKey &pStandardKey,
                    QWidget *pParent)
{
    // Create and return an action

    QAction *res = new QAction(pParent);

    res->setShortcut(pStandardKey);

    return res;
}

//==============================================================================

QFrame * newLineWidget(const bool &pHorizontal, const QColor &pColor,
                       QWidget *pParent)
{
    // Create and return a 'real' line widget, i.e. one which is 1 pixel wide,
    // using a QFrame widget

    QFrame *res = new QFrame(pParent);

    res->setStyleSheet(QString("QFrame {"
                               "    border: 1px solid rgb(%1, %2, %3);"
                               "}").arg(QString::number(pColor.red()),
                                        QString::number(pColor.green()),
                                        QString::number(pColor.blue())));

    if (pHorizontal) {
        res->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        res->setFixedHeight(1);
    } else {
        res->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        res->setFixedWidth(1);
    }

    return res;
}

//==============================================================================

QFrame * newLineWidget(const bool &pHorizontal, QWidget *pParent)
{
    // Create and return a 'real' horizontal line widget

    return newLineWidget(pHorizontal, borderColor(), pParent);
}

//==============================================================================

QFrame * newLineWidget(const QColor &pColor, QWidget *pParent)
{
    // Create and return a 'real' horizontal line widget

    return newLineWidget(true, pColor, pParent);
}

//==============================================================================

QFrame * newLineWidget(QWidget *pParent)
{
    // Create and return a 'real' horizontal line widget

    return newLineWidget(true, borderColor(), pParent);
}

//==============================================================================

QLabel * newLabel(const QString &pText, const double &pFontPercentage,
                  const bool &pBold, const bool &pItalic,
                  const Qt::Alignment &pAlignment, QWidget *pParent)
{
    // Create and return a label, after having set some of its properties

    QLabel *res = new QLabel(pText, pParent);

    QFont font = res->font();

    font.setBold(pBold);
    font.setItalic(pItalic);
    font.setPointSize(pFontPercentage*font.pointSize());

    res->setAlignment(pAlignment);
    res->setFont(font);

    // Prevent Qt from associating a context menu with the label (something
    // which it does automatically when a label is a link...)

    res->setContextMenuPolicy(Qt::NoContextMenu);

    // Return our link

    return res;
}

//==============================================================================

QLabel * newLabel(const QString &pText, const double &pFontPercentage,
                  const bool &pBold, const bool &pItalic, QWidget *pParent)
{
    // Create and return a label

    return newLabel(pText, pFontPercentage, pBold, pItalic,
                    Qt::AlignLeft|Qt::AlignVCenter, pParent);
}

//==============================================================================

QLabel * newLabel(const QString &pText, const double &pFontPercentage,
                  const bool &pBold, QWidget *pParent)
{
    // Create and return a label

    return newLabel(pText, pFontPercentage, pBold, false,
                    Qt::AlignLeft|Qt::AlignVCenter, pParent);
}

//==============================================================================

QLabel * newLabel(const QString &pText, const double &pFontPercentage,
                  QWidget *pParent)
{
    // Create and return a label

    return newLabel(pText, pFontPercentage, false, false,
                    Qt::AlignLeft|Qt::AlignVCenter, pParent);
}

//==============================================================================

QLabel * newLabel(const QString &pText, QWidget *pParent)
{
    // Create and return a label

    return newLabel(pText, 1.0, false, false, Qt::AlignLeft|Qt::AlignVCenter,
                    pParent);
}

//==============================================================================

void showEnableAction(QAction *pAction, const bool &pVisible,
                      const bool &pEnabled)
{
    // Show/enable or hide/disable the given action

    pAction->setVisible(pVisible);
    pAction->setEnabled(pVisible && pEnabled);
}

//==============================================================================

void showEnableWidget(QWidget *pWidget, const bool &pVisible,
                      const bool &pEnabled)
{
    // Show/enable or hide/disable the given widget

    pWidget->setVisible(pVisible);
    pWidget->setEnabled(pVisible && pEnabled);
}

//==============================================================================

void updateColors()
{
    // Retrieve the colour used for a 'normal' border
    // Note #1: we use a QStackedWidget object and retrieve the colour of the
    //          pixel which is in the middle of the right border...
    // Note #2: we don't rely on the top border because it may be rendered in a
    //          special way. Also, we don't rely on a corner as such in case
    //          it's rendered as a rounded corner...
    // Note #3: on OS X, our widget must be shown otherwise, the retrieved
    //          border colour will be black. So we show it off screen, even on
    //          Windows and Linux, in case their behaviour was to change in the
    //          future...

    // Create our widget and show it off screen

    QStackedWidget stackedWidget;

    stackedWidget.setFrameShape(QFrame::StyledPanel);

    stackedWidget.move(-2*stackedWidget.width(), -2*stackedWidget.height());
    stackedWidget.show();

    // Render the widget to an image

    QImage image = QImage(stackedWidget.size(),
                          QImage::Format_ARGB32_Premultiplied);

    stackedWidget.render(&image);

    // Retrieve the colour we are after

    QColor borderColor = QColor(image.pixel(image.width()-1, 0.5*image.height()));

    // Use our settings to keep track of the colour

    QSettings settings(SettingsOrganization, SettingsApplication);

    settings.beginGroup(SettingsGlobal);
        settings.setValue(SettingsBorderColor, borderColor);
    settings.endGroup();

    // Retrieve some other colours

    settings.beginGroup(SettingsGlobal);
        settings.setValue(SettingsBaseColor, qApp->palette().color(QPalette::Base));
        settings.setValue(SettingsHighlightColor, qApp->palette().color(QPalette::Highlight));
        settings.setValue(SettingsShadowColor, qApp->palette().color(QPalette::Shadow));
        settings.setValue(SettingsWindowColor, qApp->palette().color(QPalette::Window));
    settings.endGroup();
}

//==============================================================================

QColor specificColor(const QString &pColor)
{
    // Return a specific colour

    QColor res;
    QSettings settings(SettingsOrganization, SettingsApplication);

    settings.beginGroup(SettingsGlobal);
        res = settings.value(pColor).value<QColor>();
    settings.endGroup();

    return res;
}

//==============================================================================

QColor baseColor()
{
    // Return the base colour
    // Note: we retrieve it from our settings, which is updated by our plugin
    //       itself (see CorePlugin::changeEvent())...

    return specificColor(SettingsBaseColor);
}

//==============================================================================

QColor borderColor()
{
    // Return the border colour
    // Note: we retrieve it from our settings, which is updated by our plugin
    //       itself (see CorePlugin::retrieveBorderColor())...

    return specificColor(SettingsBorderColor);
}

//==============================================================================

QColor highlightColor()
{
    // Return the highlight colour
    // Note: we retrieve it from our settings, which is updated by our plugin
    //       itself (see CorePlugin::changeEvent())...

    return specificColor(SettingsHighlightColor);
}

//==============================================================================

QColor shadowColor()
{
    // Return the shadow colour
    // Note: we retrieve it from our settings, which is updated by our plugin
    //       itself (see CorePlugin::changeEvent())...

    return specificColor(SettingsShadowColor);
}

//==============================================================================

QColor windowColor()
{
    // Return the window colour
    // Note: we retrieve it from our settings, which is updated by our plugin
    //       itself (see CorePlugin::changeEvent())...

    return specificColor(SettingsWindowColor);
}

//==============================================================================

QColor lockedColor(const QColor &pColor)
{
    // Return the resulting locked version of the given colour

    int r = pColor.red();
    int g = pColor.green();
    int b = pColor.blue();

    static const int lr = 255;
    static const int lg = 0;
    static const int lb = 0;

    static const double alpha = 0.05;
    static const double oneMinusAlpha = 1.0-alpha;

    return QColor(alpha*lr+oneMinusAlpha*r,
                  alpha*lg+oneMinusAlpha*g,
                  alpha*lb+oneMinusAlpha*b);
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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

#include "centralwidget.h"
#include "corecliutils.h"
#include "commonwidget.h"
#include "coreguiutils.h"
#include "filemanager.h"

//==============================================================================

#include <QAction>
#include <QApplication>
#include <QBuffer>
#include <QDesktopWidget>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFrame>
#include <QGraphicsColorizeEffect>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QLayout>
#include <QMainWindow>
#include <QMenu>
#include <QPainter>
#include <QPushButton>
#include <QSettings>
#include <QSizePolicy>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

#include "coreguiutils.cpp.inl"

//==============================================================================

CentralWidget * centralWidget()
{
    // Retrieve and return our central widget

    static bool firstTime = true;
    static CentralWidget *res = 0;

    if (firstTime) {
        foreach (QObject *object, mainWindow()->children()) {
            if (!strcmp(object->metaObject()->className(), "OpenCOR::Core::CentralWidget")) {
                res = dynamic_cast<CentralWidget *>(object);

                break;
            }
        }

        firstTime = false;
    }

    Q_ASSERT(res);

    return res;
}

//==============================================================================

bool readFileContentsFromUrlWithBusyWidget(const QString &pUrl,
                                           QByteArray &pFileContents,
                                           QString *pErrorMessage)
{
    // Read the contents of the file, which URL is given, showing our busy
    // widget

    centralWidget()->showBusyWidget();

    bool res = readFileContentsFromUrl(pUrl, pFileContents, pErrorMessage);

    centralWidget()->hideBusyWidget();

    return res;
}

//==============================================================================

bool readFileContentsFromUrlWithBusyWidget(const QString &pUrl,
                                           QString &pFileContents,
                                           QString *pErrorMessage)
{
    // Read the contents of the file, which URL is given

    QByteArray fileContents = QByteArray();

    bool res = readFileContentsFromUrlWithBusyWidget(pUrl, fileContents, pErrorMessage);

    pFileContents = fileContents;

    return res;
}

//==============================================================================

QString allFilters(const QStringList &pFilters)
{
    QStringList filters = pFilters;

    filters.sort();

    return  QObject::tr("All Files")
           +" (*"
#ifdef Q_OS_WIN
            ".*"
#endif
            ")"
           +(filters.isEmpty()?QString():";;"+filters.join(";;"));
}

//==============================================================================

QString getOpenFileName(const QString &pCaption, const QStringList &pFilters,
                        QString *pSelectedFilter)
{
    // Retrieve and return an open file name
    // Note: normally, we would rely on QFileDialog::getOpenFileName() to
    //       retrieve an open file name, but then we wouldn't be able to handle
    //       the case where the user cancels his/her action, so instead we
    //       create and execute our own QFileDialog object...

    QFileDialog dialog(qApp->activeWindow(), pCaption, activeDirectory(),
                       allFilters(pFilters));

    dialog.setFileMode(QFileDialog::ExistingFile);

    if (pSelectedFilter && !pSelectedFilter->isEmpty())
        dialog.selectNameFilter(*pSelectedFilter);

    if (dialog.exec() == QDialog::Accepted) {
        QString res = Core::nativeCanonicalFileName(dialog.selectedFiles().first());

        if (!res.isEmpty()) {
            // We have retrieved an open file name, so keep track of the folder
            // in which it is
            // Note: normally, we would use QFileInfo::canonicalPath(), but this
            //       requires an existing file, so instead we use
            //       QFileInfo::path()...

            setActiveDirectory(QFileInfo(res).path());
        }

        return res;
    } else {
        return QString();
    }
}

//==============================================================================

QStringList getOpenFileNames(const QString &pCaption,
                             const QStringList &pFilters,
                             QString *pSelectedFilter)
{
    // Retrieve and return one or several open file names
    // Note: normally, we would rely on QFileDialog::getOpenFileNames() to
    //       retrieve one or several open file names, but then we wouldn't be
    //       able to handle the case where the user cancels his/her action, so
    //       instead we create and execute our own QFileDialog object...

    QFileDialog dialog(qApp->activeWindow(), pCaption, activeDirectory(),
                       allFilters(pFilters));

    dialog.setFileMode(QFileDialog::ExistingFiles);

    if (pSelectedFilter && !pSelectedFilter->isEmpty())
        dialog.selectNameFilter(*pSelectedFilter);

    if (dialog.exec() == QDialog::Accepted) {
        QStringList res = Core::nativeCanonicalFileNames(dialog.selectedFiles());

        if (!res.isEmpty()) {
            // We have retrieved at least one open file name, so keep track of
            // the folder in which it is
            // Note #1: we use the last open file name to determine the folder
            //          that is to be remembered since on Windows 7, at least,
            //          it's possible to search for files from within the file
            //          dialog, and the last file name should be the one we are
            //          most 'interested' in...
            // Note #2: normally, we would use QFileInfo::canonicalPath(), but
            //          this requires an existing file, so instead we use
            //          QFileInfo::path()...

            setActiveDirectory(QFileInfo(res.last()).path());
        }

        return res;
    } else {
        return QStringList();
    }
}

//==============================================================================

QString getSaveFileName(const QString &pCaption, const QString &pFileName,
                        const QStringList &pFilters, QString *pSelectedFilter)
{
    // Retrieve and return a save file name
    // Note: normally, we would rely on QFileDialog::getSaveFileName() to
    //       retrieve a save file name, but then we wouldn't be able to handle
    //       the case where a user cancels his/her action, so instead we create
    //       and execute our own QFileDialog object...

    QFileInfo fileInfo = pFileName;
    QFileDialog dialog(qApp->activeWindow(), pCaption,
                       !fileInfo.canonicalPath().compare(".")?
                           activeDirectory()+QDir::separator()+fileInfo.fileName():
                           pFileName,
                       allFilters(pFilters));

    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setOption(QFileDialog::DontConfirmOverwrite);

    forever {
        if (pSelectedFilter && !pSelectedFilter->isEmpty())
            dialog.selectNameFilter(*pSelectedFilter);

        if (dialog.exec() != QDialog::Accepted)
            break;

        if (pSelectedFilter)
            *pSelectedFilter = dialog.selectedNameFilter();

        QString res = Core::nativeCanonicalFileName(dialog.selectedFiles().first());

        // Make sure that we have got a save file name

        if (!res.isEmpty()) {
            // We have retrieved a save file name, so keep track of the folder
            // in which it is or it is to be
            // Note: normally, we would use QFileInfo::canonicalPath(), but this
            //       requires an existing file, so instead we use
            //       QFileInfo::path()...

            QFileInfo resInfo = res;

            setActiveDirectory(resInfo.path());

            // Check whether the save file already exists and is opened

            if (Core::FileManager::instance()->file(res)) {
                warningMessageBox(pCaption,
                                  QObject::tr("<strong>%1</strong> already exists and is opened.").arg(res));

                continue;
            }

            // Check whether the save file already exists

            if (   resInfo.exists()
                && questionMessageBox(pCaption,
                                      QObject::tr("<strong>%1</strong> already exists. Do you want to overwrite it?").arg(res)) == QMessageBox::No) {
                continue;
            }
        }

        return res;
    }

    return QString();
}

//==============================================================================

QString getDirectory(const QString &pCaption, const QString &pDirName,
                     const bool &pEmptyDir)
{
    // Retrieve and return the name of an existing directory
    // Note: normally, we would rely on QFileDialog::getExistingDirectory() to
    //       retrieve the path of an existing directory, but then we wouldn't be
    //       able to handle the case where the user cancels his/her action, so
    //       instead we create and execute our own QFileDialog object...

    QFileDialog dialog(qApp->activeWindow(), pCaption,
                       pDirName.isEmpty()?activeDirectory():pDirName);

    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setOption(QFileDialog::ShowDirsOnly);

    forever {
        if (dialog.exec() != QDialog::Accepted)
            break;

        QString res = Core::nativeCanonicalDirName(dialog.selectedFiles().first());

        if (!res.isEmpty()) {
            // We have retrieved a file name, so update our active directory

            setActiveDirectory(res);

            // Check whether the directory should be empty

            if (pEmptyDir && !isEmptyDirectory(res)) {
                warningMessageBox(pCaption,
                                  QObject::tr("Please choose an empty directory."));

                continue;
            }
        }

        return res;
    }

    return QString();
}

//==============================================================================

QString getEmptyDirectory(const QString &pCaption, const QString &pDirName)
{
    // Retrieve and return the name of an empty directory

    return getDirectory(pCaption, pDirName, true);
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

    res->setObjectName("menu"+pName.left(1).toUpper()+pName.right(pName.length()-1));

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

QAction * newAction(const bool &pCheckable, const QIcon &pIcon,
                    QWidget *pParent)
{
    // Create and return an action

    QAction *res = new QAction(pParent);

    res->setCheckable(pCheckable);
    res->setIcon(pIcon);

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

QAction * newAction(QWidget *pParent)
{
    // Create and return an action

    return new QAction(pParent);
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

QString iconDataUri(const QIcon &pIcon, const int &pWidth, const int &pHeight,
                    const QIcon::Mode &pMode)
{
    // Convert an icon, which resource name is given, to a data URI, after
    // having resized it, if requested

    if (pIcon.isNull())
        return QString();

    QByteArray data;
    QBuffer buffer(&data);

    buffer.open(QIODevice::WriteOnly);

    pIcon.pixmap(pWidth, pHeight, pMode).save(&buffer, "PNG");

    return QString("data:image/png;base64,%1").arg(QString(data.toBase64()));
}

//==============================================================================

QString iconDataUri(const QString &pIcon, const int &pWidth, const int &pHeight,
                    const QIcon::Mode &pMode)
{
    // Convert an icon, which resource name is given, to a data URI, after
    // having resized it, if requested

    return iconDataUri(QIcon(pIcon), pWidth, pHeight, pMode);
}

//==============================================================================

QIcon tintedIcon(const QIcon &pIcon, const int &pWidth, const int &pHeight,
                 const QColor &pColor)
{
    // Create and return a tinted icon using the given icon and colour

    QGraphicsScene scene(0, 0, pWidth, pHeight);
    QGraphicsPixmapItem pixmapItem;
    QGraphicsColorizeEffect effect;

    effect.setColor(pColor);

    pixmapItem.setGraphicsEffect(&effect);
    pixmapItem.setPixmap(pIcon.pixmap(pWidth, pHeight));

    scene.addItem(&pixmapItem);

    QImage image(pWidth, pHeight, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(image.rect(), Qt::transparent);

    scene.render(&painter);

    return QPixmap::fromImage(image);
}

//==============================================================================

QIcon tintedIcon(const QString &pIcon, const int &pWidth, const int &pHeight,
                 const QColor &pColor)
{
    // Create and return a tinted icon using the given icon and colour

    return tintedIcon(QIcon(pIcon), pWidth, pHeight, pColor);
}

//==============================================================================

QIcon overlayedIcon(const QIcon &pBaseIcon, const QIcon &pOverlayIcon,
                    const int &pBaseWidth, const int &pBaseHeight,
                    const int &pOverlayLeft, const int &pOverlayTop,
                    const int &pOverlayWidth, const int &pOverlayHeight)
{
    // Create and return an overlayed icon using the given base and overlay
    // icons

    QImage image(pBaseWidth, pBaseHeight, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(image.rect(), Qt::transparent);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawPixmap(0, 0, pBaseWidth, pBaseWidth,
                       pBaseIcon.pixmap(pBaseWidth, pBaseWidth));

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawPixmap(pOverlayLeft, pOverlayTop, pOverlayWidth, pOverlayHeight,
                       pOverlayIcon.pixmap(pOverlayWidth, pOverlayHeight));

    return QPixmap::fromImage(image);
}

//==============================================================================

QIcon overlayedIcon(const QString &pBaseIcon, const QIcon &pOverlayIcon,
                    const int &pBaseWidth, const int &pBaseHeight,
                    const int &pOverlayLeft, const int &pOverlayTop,
                    const int &pOverlayWidth, const int &pOverlayHeight)
{
    // Create and return an overlayed icon using the given base and overlay
    // icons

    return overlayedIcon(QIcon(pBaseIcon), pOverlayIcon, pBaseWidth, pBaseHeight,
                         pOverlayLeft, pOverlayTop, pOverlayWidth, pOverlayHeight);
}

//==============================================================================

QIcon overlayedIcon(const QIcon &pBaseIcon, const QString &pOverlayIcon,
                    const int &pBaseWidth, const int &pBaseHeight,
                    const int &pOverlayLeft, const int &pOverlayTop,
                    const int &pOverlayWidth, const int &pOverlayHeight)
{
    // Create and return an overlayed icon using the given base and overlay
    // icons

    return overlayedIcon(pBaseIcon, QIcon(pOverlayIcon), pBaseWidth, pBaseHeight,
                         pOverlayLeft, pOverlayTop, pOverlayWidth, pOverlayHeight);
}

//==============================================================================

QIcon overlayedIcon(const QString &pBaseIcon, const QString &pOverlayIcon,
                    const int &pBaseWidth, const int &pBaseHeight,
                    const int &pOverlayLeft, const int &pOverlayTop,
                    const int &pOverlayWidth, const int &pOverlayHeight)
{
    // Create and return an overlayed icon using the given base and overlay
    // icons

    return overlayedIcon(QIcon(pBaseIcon), QIcon(pOverlayIcon), pBaseWidth, pBaseHeight,
                         pOverlayLeft, pOverlayTop, pOverlayWidth, pOverlayHeight);
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

QColor lockedColor(const QColor &pColor)
{
    // Return the resulting locked version of the given colour

    int red = pColor.red();
    int green = pColor.green();
    int blue = pColor.blue();

    enum {
        lockedRed = 255,
        lockedGreen = 0,
        lockedBlue = 0
    };

    static const double Alpha = 0.05;
    static const double OneMinusAlpha = 1.0-Alpha;

    return QColor(Alpha*lockedRed+OneMinusAlpha*red,
                  Alpha*lockedGreen+OneMinusAlpha*green,
                  Alpha*lockedBlue+OneMinusAlpha*blue);
}

//==============================================================================

QStringList filters(const FileTypeInterfaces &pFileTypeInterfaces,
                    const bool &pCheckMimeTypes, const QStringList &pMimeTypes)
{
    // Convert and return as a list of strings the filters corresponding to the
    // given file type interfaces, using the given MIME types, if any

    QStringList res = QStringList();

    foreach (FileTypeInterface *fileTypeInterface, pFileTypeInterfaces) {
        if (!pCheckMimeTypes || pMimeTypes.contains(fileTypeInterface->mimeType()))
            res << fileTypeInterface->fileTypeDescription()+" (*."+fileTypeInterface->fileExtension()+")";
    }

    return res;
}

//==============================================================================

QStringList filters(const FileTypeInterfaces &pFileTypeInterfaces)
{
    // Convert and return as a list of strings the filters corresponding to the
    // given file type interfaces

    return filters(pFileTypeInterfaces, false, QStringList());
}

//==============================================================================

QStringList filters(const FileTypeInterfaces &pFileTypeInterfaces,
                    const QStringList &pMimeTypes)
{
    // Convert and return as a list of strings the filters corresponding to the
    // given file type interfaces, using the given MIME types

    return filters(pFileTypeInterfaces, true, pMimeTypes);
}

//==============================================================================

bool opencorActive()
{
    // Return whether OpenCOR is active
    // Note: we only consider OpenCOR to be active if the main window or one of
    //       its dockable windows is active. In other words, if a dialog is
    //       opened, then we don't consider OpenCOR active since it could
    //       disturb our user's workflow...

    return     qApp->activeWindow()
           && !qApp->activeModalWidget()
           && !qApp->activePopupWidget();
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

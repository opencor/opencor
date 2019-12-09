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

#include "centralwidget.h"
#include "commonwidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "filemanager.h"

//==============================================================================

#include <QAction>
#include <QApplication>
#include <QBuffer>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QFrame>
#include <QGraphicsColorizeEffect>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QLayout>
#include <QMainWindow>
#include <QMenu>
#include <QPainter>
#include <QPushButton>
#include <QScreen>
#include <QSettings>
#include <QSizePolicy>
#include <QStandardItemModel>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

#include "guiutils.cpp.inl"

//==============================================================================

CentralWidget * centralWidget()
{
    // Make sure that we have a main window, i.e. that we are running the GUI
    // version of OpenCOR

    if (mainWindow() == nullptr) {
        return nullptr;
    }

    // Retrieve and return our central widget

    static bool firstTime = true;
    static CentralWidget *res = nullptr;

    if (firstTime) {
        for (const auto &child : mainWindow()->children()) {
            if (strcmp(child->metaObject()->className(), "OpenCOR::Core::CentralWidget") == 0) {
                res = qobject_cast<CentralWidget *>(child);

                break;
            }
        }

        firstTime = false;
    }

    Q_ASSERT(res);

    return res;
}

//==============================================================================

void showCentralBusyWidget()
{
    // Show our central busy widget, if possible

    CentralWidget *centralWidget = Core::centralWidget();

    if (centralWidget != nullptr) {
        centralWidget->showBusyWidget();
    }
}

//==============================================================================

void showCentralProgressBusyWidget()
{
    // Show our central progress busy widget, if possible

    CentralWidget *centralWidget = Core::centralWidget();

    if (centralWidget != nullptr) {
        centralWidget->showProgressBusyWidget();
    }
}

//==============================================================================

void setCentralBusyWidgetProgress(double pProgress)
{
    // Set our central busy widget progress, if possible

    CentralWidget *centralWidget = Core::centralWidget();

    if (centralWidget != nullptr) {
        centralWidget->setBusyWidgetProgress(pProgress);
    }
}

//==============================================================================

void hideCentralBusyWidget()
{
    // Hide our central busy widget, if possible

    CentralWidget *centralWidget = Core::centralWidget();

    if (centralWidget != nullptr) {
        centralWidget->hideBusyWidget();
    }
}

//==============================================================================

Plugin * currentViewPlugin()
{
    // Retrieve and return our current view plugin

    CentralWidget *centralWidget = Core::centralWidget();

    if (centralWidget != nullptr) {
        return centralWidget->currentViewPlugin();
    }

    return nullptr;
}

//==============================================================================

QString currentFileName()
{
    // Return our current file name

    CentralWidget *centralWidget = Core::centralWidget();

    if (centralWidget != nullptr) {
        return centralWidget->currentFileName();
    }

    return {};
}

//==============================================================================

QString allFilters(const QStringList &pFilters)
{
    // Return all the filters as a string

    QStringList filters = pFilters;

    filters.sort(Qt::CaseInsensitive);

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

    if ((pSelectedFilter != nullptr) && !pSelectedFilter->isEmpty()) {
        dialog.selectNameFilter(*pSelectedFilter);
    }

    if (dialog.exec() == QDialog::Accepted) {
        QString res = canonicalFileName(dialog.selectedFiles().first());

        if (!res.isEmpty()) {
            // We have retrieved an open file name, so keep track of the folder
            // in which it is
            // Note: normally, we would use QFileInfo::canonicalPath(), but this
            //       requires an existing file, so instead we use
            //       QFileInfo::path()...

            setActiveDirectory(QFileInfo(res).path());
        }

        return res;
    }

    return {};
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

    if ((pSelectedFilter != nullptr) && !pSelectedFilter->isEmpty()) {
        dialog.selectNameFilter(*pSelectedFilter);
    }

    if (dialog.exec() == QDialog::Accepted) {
        QStringList res = canonicalFileNames(dialog.selectedFiles());

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

        // There may be duplicates (in case we selected some symbolic links), so
        // remove them

        res.removeDuplicates();

        return res;
    }

    return {};
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

    static const QString Dot = ".";

    QFileInfo fileInfo = pFileName;
    QFileDialog dialog(qApp->activeWindow(), pCaption,
                       (fileInfo.canonicalPath() == Dot)?
                           activeDirectory()+"/"+fileInfo.fileName():
                           pFileName,
                       allFilters(pFilters));

    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setOption(QFileDialog::DontConfirmOverwrite);

    forever {
        if ((pSelectedFilter != nullptr) && !pSelectedFilter->isEmpty()) {
            dialog.selectNameFilter(*pSelectedFilter);
        }

        if (dialog.exec() != QDialog::Accepted) {
            break;
        }

        if (pSelectedFilter != nullptr) {
            *pSelectedFilter = dialog.selectedNameFilter();
        }

        QString res = canonicalFileName(dialog.selectedFiles().first());

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

            if (FileManager::instance()->file(res) != nullptr) {
                warningMessageBox(pCaption,
                                  QObject::tr("<strong>%1</strong> already exists and is opened.").arg(QDir::toNativeSeparators(res)));

                continue;
            }

            // Check whether the save file already exists

            if (   resInfo.exists()
                && questionMessageBox(pCaption,
                                      QObject::tr("<strong>%1</strong> already exists. Do you want to overwrite it?").arg(QDir::toNativeSeparators(res))) == QMessageBox::No) {
                continue;
            }
        }

        return res;
    }

    return {};
}

//==============================================================================

QString getSaveFileName(const QString &pCaption, const QStringList &pFilters,
                        QString *pSelectedFilter)
{
    // Retrieve and return a save file name

    return getSaveFileName(pCaption, QString(), pFilters, pSelectedFilter);
}

//==============================================================================

QString getDirectory(const QString &pCaption, const QString &pDirName,
                     bool pEmptyDir)
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
        if (dialog.exec() != QDialog::Accepted) {
            break;
        }

        QString res = canonicalDirName(dialog.selectedFiles().first());

        if (!res.isEmpty()) {
            // We have retrieved a file name, so update our active directory

            setActiveDirectory(res);

            // Check whether the directory is writable

            if (!Core::isDirectory(res)) {
                Core::warningMessageBox(pCaption,
                                        QObject::tr("Please choose a writable directory."));

                continue;
            }

            // Check whether the directory should be and is empty

            if (pEmptyDir && !isEmptyDirectory(res)) {
                warningMessageBox(pCaption,
                                  QObject::tr("Please choose an empty directory."));

                continue;
            }
        }

        return res;
    }

    return {};
}

//==============================================================================

QString getEmptyDirectory(const QString &pCaption, const QString &pDirName)
{
    // Retrieve and return the name of an empty directory

    return getDirectory(pCaption, pDirName, true);
}

//==============================================================================

QString CORE_EXPORT guiOpenFile(const QString &pFileName, File::Type pType,
                                const QString &pUrl, bool pShowWarning)
{
    // Open the given file

    CentralWidget *centralWidget = Core::centralWidget();

    if (centralWidget != nullptr) {
        return centralWidget->openFile(pFileName, pType, pUrl, pShowWarning);
    }

    return QObject::tr("there is no central widget, so the file cannot be opened.");
    // Note: we should never reach this point...
}

//==============================================================================

QString guiOpenRemoteFile(const QString &pUrl, bool pShowWarning)
{
    // Open the given remote file

    CentralWidget *centralWidget = Core::centralWidget();

    if (centralWidget != nullptr) {
        return centralWidget->openRemoteFile(pUrl, pShowWarning);
    }

    return QObject::tr("there is no central widget, so the remote file cannot be opened.");
    // Note: we should never reach this point...
}

//==============================================================================

bool guiCloseFile(const QString &pFileName)
{
    // Close the given file

    CentralWidget *centralWidget = Core::centralWidget();

    if (centralWidget != nullptr) {
        return centralWidget->closeFile(pFileName);
    }

    return false;
}

//==============================================================================

void setFocusTo(QWidget *pWidget)
{
    // Give the focus to pWidget, but then revert the focus back to whoever had
    // it before, if needed

    if (pWidget == nullptr) {
        return;
    }

    QWidget *focusedWidget = qApp->activeWindow()?
                                 qApp->activeWindow()->focusWidget():
                                 nullptr;

    if (   (focusedWidget == nullptr)
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

    auto res = new QMenu(pParent);

    res->setObjectName("menu"+pName.left(1).toUpper()+pName.right(pName.length()-1));

    return res;
}

//==============================================================================

QMenu * newMenu(const QIcon &pIcon, QWidget *pParent)
{
    // Create and return a menu

    auto res = new QMenu(pParent);

    res->menuAction()->setIcon(pIcon);

    return res;
}

//==============================================================================

QAction * newAction(bool pCheckable, const QIcon &pIcon,
                    const QKeySequence &pKeySequence, QWidget *pParent)
{
    // Create and return an action

    auto res = new QAction(pParent);

    res->setCheckable(pCheckable);
    res->setIcon(pIcon);
    res->setShortcut(pKeySequence);

    return res;
}

//==============================================================================

QAction * newAction(bool pCheckable, const QIcon &pIcon, QWidget *pParent)
{
    // Create and return an action

    auto res = new QAction(pParent);

    res->setCheckable(pCheckable);
    res->setIcon(pIcon);

    return res;
}

//==============================================================================

QAction * newAction(bool pCheckable, const QKeySequence &pKeySequence,
                    QWidget *pParent)
{
    // Create and return an action

    auto res = new QAction(pParent);

    res->setCheckable(pCheckable);
    res->setShortcut(pKeySequence);

    return res;
}

//==============================================================================

QAction * newAction(bool pCheckable, QWidget *pParent)
{
    // Create and return an action

    auto res = new QAction(pParent);

    res->setCheckable(pCheckable);

    return res;
}

//==============================================================================

QAction * newAction(const QIcon &pIcon,
                    const QList<QKeySequence> &pKeySequences, QWidget *pParent)
{
    // Create and return an action

    auto res = new QAction(pParent);

    res->setIcon(pIcon);
    res->setShortcuts(pKeySequences);

    return res;
}

//==============================================================================

QAction * newAction(const QIcon &pIcon, const QKeySequence &pKeySequence,
                    QWidget *pParent)
{
    // Create and return an action

    auto res = new QAction(pParent);

    res->setIcon(pIcon);
    res->setShortcut(pKeySequence);

    return res;
}

//==============================================================================

QAction * newAction(const QIcon &pIcon, QWidget *pParent)
{
    // Create and return an action

    auto res = new QAction(pParent);

    res->setIcon(pIcon);

    return res;
}

//==============================================================================

QAction * newAction(const QKeySequence &pKeySequence, QWidget *pParent)
{
    // Create and return an action

    auto res = new QAction(pParent);

    res->setShortcut(pKeySequence);

    return res;
}

//==============================================================================

QAction * newAction(const QKeySequence::StandardKey &pStandardKey,
                    QWidget *pParent)
{
    // Create and return an action

    auto res = new QAction(pParent);

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

QAction * newSeparator(QWidget *pParent)
{
    // Create and return a separator

    auto res = new QAction(pParent);

    res->setSeparator(true);

    return res;
}

//==============================================================================

QFrame * newLineWidget(bool pHorizontal, QWidget *pParent)
{
    // Create and return a line widget, i.e. one which is 1 pixel wide, using a
    // QFrame widget

    auto res = new QFrame(pParent);
    QString color = borderColor().name();

    res->setStyleSheet(QString("QFrame {"
                               "    border: 1px solid %1;"
                               "}").arg(color));

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

QFrame * newLineWidget(QWidget *pParent)
{
    // Return a new horizontal line widget

    return newLineWidget(true, pParent);
}

//==============================================================================

QString iconDataUri(const QIcon &pIcon, int pWidth, int pHeight,
                    QIcon::Mode pMode)
{
    // Convert and return an icon, which resource name and size are given, to a
    // data URI

    if (pIcon.isNull()) {
        return {};
    }

    QByteArray data;
    QBuffer buffer(&data);

    buffer.open(QIODevice::WriteOnly);

    pIcon.pixmap(pWidth, pHeight, pMode).save(&buffer, "PNG");

    return QString("data:image/png;base64,%1").arg(QString(data.toBase64()));
}

//==============================================================================

QString iconDataUri(const QString &pIcon, int pWidth, int pHeight,
                    QIcon::Mode pMode)
{
    // Convert and return an icon, which resource name and size are given, to a
    // data URI

    return iconDataUri(QIcon(pIcon), pWidth, pHeight, pMode);
}

//==============================================================================

QIcon standardIcon(QStyle::StandardPixmap pStandardIcon,
                   const QStyleOption *pOption, const QWidget *pWidget)
{
    // Retrieve the given standard icon

    return qApp->style()->standardIcon(pStandardIcon, pOption, pWidget);
}

//==============================================================================

QIcon tintedIcon(const QIcon &pIcon, const QColor &pColor)
{
    // Create and return a tinted icon using (all the sizes of) the given icon
    // and colour

    QIcon res = QIcon();

    for (const auto &size : pIcon.availableSizes()) {
        QGraphicsScene scene(0, 0, size.width(), size.height());
        QGraphicsPixmapItem pixmapItem;
        QGraphicsColorizeEffect effect;

        effect.setColor(pColor);

        pixmapItem.setGraphicsEffect(&effect);
        pixmapItem.setPixmap(pIcon.pixmap(size));

        scene.addItem(&pixmapItem);

        QImage image(size, QImage::Format_ARGB32_Premultiplied);
        QPainter painter(&image);

        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.fillRect(image.rect(), Qt::transparent);

        scene.render(&painter);

        res.addPixmap(QPixmap::fromImage(image));
    }

    return res;
}

//==============================================================================

QIcon tintedIcon(const QString &pIcon, const QColor &pColor)
{
    // Return a tinted icon using the given icon and colour

    return tintedIcon(QIcon(pIcon), pColor);
}

//==============================================================================

QIcon overlayedIcon(const QIcon &pBaseIcon, const QIcon &pOverlayIcon,
                    int pBaseWidth, int pBaseHeight, int pOverlayLeft,
                    int pOverlayTop, int pOverlayWidth, int pOverlayHeight)
{
    // Create and return an overlayed icon using the given base and overlay
    // icons
    // Note: there is a bug in QIcon::pixmap() when it comes to HiDPI screens
    //       (see https://bugreports.qt.io/browse/QTBUG-71333), hence we need to
    //       scale things...

    QImage image(pBaseWidth, pBaseHeight, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);
    double scalingFactor = 1.0/qApp->devicePixelRatio();

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(image.rect(), Qt::transparent);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawPixmap(0, 0, pBaseWidth, pBaseWidth,
                       pBaseIcon.pixmap(int(scalingFactor*pBaseWidth),
                                        int(scalingFactor*pBaseWidth)));
    painter.drawPixmap(pOverlayLeft, pOverlayTop, pOverlayWidth, pOverlayHeight,
                       pOverlayIcon.pixmap(int(scalingFactor*pOverlayWidth),
                                           int(scalingFactor*pOverlayHeight)));

    return QPixmap::fromImage(image);
}

//==============================================================================

QIcon overlayedIcon(const QString &pBaseIcon, const QIcon &pOverlayIcon,
                    int pBaseWidth, int pBaseHeight, int pOverlayLeft,
                    int pOverlayTop, int pOverlayWidth, int pOverlayHeight)
{
    // Return an overlayed icon using the given base and overlay icons

    return overlayedIcon(QIcon(pBaseIcon), pOverlayIcon, pBaseWidth, pBaseHeight,
                         pOverlayLeft, pOverlayTop, pOverlayWidth, pOverlayHeight);
}

//==============================================================================

QIcon overlayedIcon(const QIcon &pBaseIcon, const QString &pOverlayIcon,
                    int pBaseWidth, int pBaseHeight, int pOverlayLeft,
                    int pOverlayTop, int pOverlayWidth, int pOverlayHeight)
{
    // Return an overlayed icon using the given base and overlay icons

    return overlayedIcon(pBaseIcon, QIcon(pOverlayIcon), pBaseWidth, pBaseHeight,
                         pOverlayLeft, pOverlayTop, pOverlayWidth, pOverlayHeight);
}

//==============================================================================

QIcon overlayedIcon(const QString &pBaseIcon, const QString &pOverlayIcon,
                    int pBaseWidth, int pBaseHeight, int pOverlayLeft,
                    int pOverlayTop, int pOverlayWidth, int pOverlayHeight)
{
    // Return an overlayed icon using the given base and overlay icons

    return overlayedIcon(QIcon(pBaseIcon), QIcon(pOverlayIcon), pBaseWidth, pBaseHeight,
                         pOverlayLeft, pOverlayTop, pOverlayWidth, pOverlayHeight);
}

//==============================================================================

QIcon scaledIcon(const QIcon &pIcon, int pWidth, int pHeight,
                 Qt::AspectRatioMode pAspectMode, Qt::TransformationMode pMode)
{
    // Return a scaled version of the given icon

    return pIcon.pixmap(pIcon.availableSizes().first()).scaled(pWidth, pHeight,
                                                               pAspectMode,
                                                               pMode);
}

//==============================================================================

QIcon scaledIcon(const QString &pIcon, int pWidth, int pHeight,
                 Qt::AspectRatioMode pAspectMode, Qt::TransformationMode pMode)
{
    // Return a scaled version of the given icon

    return scaledIcon(QIcon(pIcon), pWidth, pHeight, pAspectMode, pMode);
}

//==============================================================================

void showEnableWidget(QWidget *pWidget, bool pVisible, bool pEnabled)
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

    return { int(Alpha*lockedRed+OneMinusAlpha*red),
             int(Alpha*lockedGreen+OneMinusAlpha*green),
             int(Alpha*lockedBlue+OneMinusAlpha*blue) };
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

} // namespace Core
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

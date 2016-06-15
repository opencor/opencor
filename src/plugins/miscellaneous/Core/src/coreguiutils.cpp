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
// GUI utilities
//==============================================================================

#include "centralwidget.h"
#include "corecliutils.h"
#include "commonwidget.h"
#include "coreguiutils.h"
#include "filemanager.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QAction>
#include <QApplication>
#include <QBuffer>
#include <QColor>
#include <QDate>
#include <QFileDialog>
#include <QFileInfo>
#include <QFont>
#include <QFrame>
#include <QIODevice>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QPalette>
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
           +(filters.isEmpty()?QString():";;"+filters.join(";;")) ;
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
            //          dialog box, and the last file name should be the one we
            //          are most 'interested' in...
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
                QMessageBox::warning(qApp->activeWindow(), pCaption,
                                     QObject::tr("<strong>%1</strong> already exists and is opened.").arg(res));

                continue;
            }

            // Check whether the save file already exists

            if (   resInfo.exists()
                && QMessageBox::question(qApp->activeWindow(), pCaption,
                                         QObject::tr("<strong>%1</strong> already exists. Do you want to overwrite it?").arg(res),
                                         QMessageBox::Yes|QMessageBox::No,
                                         QMessageBox::Yes) == QMessageBox::No) {
                continue;
            }
        }

        return res;
    }

    return QString();
}

//==============================================================================

QString getExistingDirectory(const QString &pCaption, const QString &pDirName,
                             const bool &pEmptyDir)
{
    // Retrieve and return an existing directory path
    // Note: normally, we would rely on QFileDialog::getExistingDirectory() to
    //       retrieve the path of an existing directory, but then we wouldn't be
    //       able to handle the case where the user cancels his/her action, so
    //       instead we create and execute our own QFileDialog object...

    QFileDialog dialog(qApp->activeWindow(), pCaption,
                       pDirName.isEmpty()?activeDirectory():pDirName);

    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setOption(QFileDialog::ShowDirsOnly);

    if (dialog.exec() == QDialog::Accepted) {
        QString res = Core::nativeCanonicalDirName(dialog.selectedFiles().first());

        if (!res.isEmpty()) {
            // We have retrieved a file name, so update our active directory

            setActiveDirectory(res);

            // Check whether the directory should be empty

            if (   pEmptyDir
                && QDir(res).entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries).count()) {
                QMessageBox::warning(qApp->activeWindow(), pCaption,
                                     QObject::tr("Please choose an empty directory."));

                return QString();
            }
        }

        return res;
    } else {
        return QString();
    }
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

QString iconDataUri(const QString &pIcon, const int &pWidth, const int &pHeight,
                    const QIcon::Mode &pMode)
{
    // Convert an icon, which resource name is given, to a data URI, after
    // having resized it, if requested

    QIcon icon(pIcon);

    if (icon.isNull())
        return QString();

    QByteArray data;
    QBuffer buffer(&data);
    QSize iconSize = icon.availableSizes().first();

    buffer.open(QIODevice::WriteOnly);
    icon.pixmap((pWidth == -1)?iconSize.width():pWidth,
                (pHeight == -1)?iconSize.height():pHeight,
                pMode).save(&buffer, "PNG");

    return QString("data:image/png;base64,%1").arg(QString(data.toBase64()));
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

QStringList filters(const FileTypes &pFileTypes, const bool &pCheckMimeTypes,
                    const QStringList &pMimeTypes)
{
    // Convert and return as a list of filters the given file types, using the
    // given MIME types, if required

    QStringList res = QStringList();

    foreach (FileType *fileType, pFileTypes) {
        if (!pCheckMimeTypes || pMimeTypes.contains(fileType->mimeType()))
            res << fileType->description()+" (*."+fileType->fileExtension()+")";
    }

    return res;
}

//==============================================================================

QStringList filters(const FileTypes &pFileTypes)
{
    // Convert and return as a list of filters the given file types

    return filters(pFileTypes, false, QStringList());
}

//==============================================================================

QStringList filters(const FileTypes &pFileTypes, const QStringList &pMimeTypes)
{
    // Convert and return as a list of filters the given file types, using the
    // given MIME types, if any

    return filters(pFileTypes, true, pMimeTypes);
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

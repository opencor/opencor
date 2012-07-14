//==============================================================================
// Core utilities
//==============================================================================

#include "coreutils.h"

//==============================================================================

#include <QApplication>
#include <QDate>
#include <QDir>
#include <QFile>
#include <QFrame>
#include <QLabel>
#include <QResource>
#include <QSettings>
#include <QWidget>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

QByteArray resourceAsByteArray(const QString &pResource)
{
    // Retrieve a resource as a QByteArray

    QResource resource(pResource);

    if (resource.isValid()) {
        if (resource.isCompressed())
            // The resource is compressed, so uncompress it before returning it

            return qUncompress(reinterpret_cast<const uchar *>(resource.data()),
                               resource.size());
        else
            // The resource is not compressed, so just return it after doing the
            // right conversion

            return QByteArray(reinterpret_cast<const char *>(resource.data()),
                              resource.size());
    }
    else {
        // The resource doesn't exist, so...

        return QByteArray();
    }
}

//==============================================================================

void saveResourceAs(const QString &pResource, const QString &pFilename)
{
    if (QResource(pResource).isValid()) {
        // The resource exists, so write a file with name pFilename and which
        // contents is that of the resource which is retrieved as a QByteArray

        QFile file(pFilename);

        file.open(QIODevice::ReadWrite);
        file.write(resourceAsByteArray(pResource));
        file.close();
    }
}

//==============================================================================

#ifdef Q_WS_MAC
void * instance(const QString &, void *pDefaultGlobalInstance)
#else
void * instance(const QString &pClassName, void *pDefaultGlobalInstance)
#endif
{
    // Retrieve the 'global' instance associated with a given class
    // Note: initially, the plan was to have a static instance of a given class
    //       and return its address. However, this approach doesn't work on
    //       Windows and Linux (but does on Mac OS X). Indeed, say that the Core
    //       plugin is required by two other plugins, then these two plugins
    //       won't get the same 'copy' of the Core plugin. (It seems like) each
    //       'copy' gets its own address space. (This is not the case on Mac OS
    //       X, (most likely) because of the way applications are bundled on
    //       that platform.) To address this issue, we keep track of the address
    //       of a 'global' instance using QSettings. Now, this approach works
    //       fine on both Windows and Linux, but... not on Mac OS X (!!). (It
    //       would seem that) there are some read/write conflicts (when using
    //       QSettings). These conflicts would normally be addressed using a
    //       mutex, but then we would be back to the issue of being able to
    //       share something between different plugins. So, instead, we, on Mac
    //       OS X, revert to our original plan...

#ifdef Q_WS_MAC
    return (void *) pDefaultGlobalInstance;
#else
    QSettings settings(qApp->applicationName());
    qlonglong globalInstance;

    settings.beginGroup("Instances");
        globalInstance = settings.value(pClassName, 0).toLongLong();

        if (!globalInstance) {
            // There is no 'global' instance associated with the given class, so
            // use the class's default 'global' instance we were given

            globalInstance = (qlonglong) pDefaultGlobalInstance;

            settings.setValue(pClassName, globalInstance);
        }
    settings.endGroup();

    // Return the class's 'global' instance

    return (void *) globalInstance;
#endif
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

    pWidget->setFocus();

    if (   focusedWidget
        && (pWidget->parentWidget() != focusedWidget->parentWidget()))
        // The current and previously focused widgets don't share the same
        // parent, so revert the focus back to the previously focused widget

        focusedWidget->setFocus();
}

//==============================================================================

QString nativeCanonicalFileName(const QString &pFileName)
{
    // Return a native and canonical version of the file's name

    return QDir::toNativeSeparators(QFileInfo(pFileName).canonicalFilePath());
}

//==============================================================================

QFrame * newLineWidget(QWidget *pParent, const bool &pHorizontal)
{
    // Return a 'real' line widget, i.e. one which is 1 pixel wide

    QFrame *res = new QFrame(pParent);

    res->setFrameShape(QFrame::StyledPanel);

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

QLabel * newLabel(QWidget *pParent, const QString &pText,
                  const double &pFontPercentage,
                  const bool &pBold, const bool &pItalic,
                  const Qt::Alignment &pAlignment)
{
    // Create and return a label, allowing to set some of its properties

    QLabel *res = new QLabel(pText, pParent);

    QFont font = res->font();

    font.setBold(pBold);
    font.setItalic(pItalic);
    font.setPointSize(pFontPercentage*font.pointSize());

    res->setAlignment(pAlignment);
    res->setFont(font);

    return res;
}

//==============================================================================

QLabel * newLabelLink(QWidget *pParent, const QString &pText,
                      const double &pFontPercentage,
                      const bool &pBold, const bool &pItalic,
                      const Qt::Alignment &pAlignment)
{
    // Create a label link, allowing to set set some of its properties

    QLabel *res = newLabel(pParent, pText, pFontPercentage,
                           pBold, pItalic, pAlignment);

    // Prevent Qt from associating a context menu with the label (something
    // which it does automatically when a label is a link...)

    res->setContextMenuPolicy(Qt::NoContextMenu);

    // Return our label link

    return res;
}

//==============================================================================

QString copyright()
{
    // Return a generic copyright

    return QObject::tr("Copyright")+" ©2011-"+QString::number(QDate::currentDate().year());
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

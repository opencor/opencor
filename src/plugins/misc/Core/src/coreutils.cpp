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
#include <QTextEdit>
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

bool saveResourceAs(const QString &pResource, const QString &pFilename)
{
    if (QResource(pResource).isValid()) {
        // The resource exists, so write a file with name pFilename and which
        // contents is that of the resource which is retrieved as a QByteArray

        QFile file(pFilename);

        if (file.open(QIODevice::ReadWrite)) {
            QByteArray resource = resourceAsByteArray(pResource);
            bool res;

            res = file.write(resource) == resource.size();

            file.close();

            // Remove the newly created file in case not all the resource was
            // written to it

            if (!res)
                file.remove();

            return res;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

//==============================================================================

void * instance(const QString &pClassName, void *pDefaultGlobalInstance)
{
    // Retrieve the 'global' instance associated with a given class
    // Note: initially, the plan was to have a static instance of a given class
    //       and return its address. However, this approach doesn't work on
    //       Windows and Linux (but does on OS X). Indeed, say that the Core
    //       plugin is required by two other plugins, then these two plugins
    //       won't get the same 'copy' of the Core plugin. (It seems like) each
    //       'copy' gets its own address space. (This is not the case on OS X,
    //       (most likely) because of the way applications are bundled on that
    //       platform.) To address this issue, we keep track of the address of a
    //       'global' instance using QSettings. Now, this approach works fine on
    //       both Windows and Linux, but... not on OS X (!!). (It would seem
    //       that) there are some read/write conflicts (when using QSettings).
    //       These conflicts would normally be addressed using a mutex, but then
    //       we would be back to the issue of being able to share something
    //       between different plugins. So, instead, we, on OS X, revert to our
    //       original plan...

#ifdef Q_WS_MAC
    Q_UNUSED(pClassName);

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

QFrame * newLineWidget(const bool &pHorizontal, QWidget *pParent)
{
    // Return a 'real' line widget, i.e. one which is 1 pixel wide, using a
    // QFrame widget

#ifdef Q_WS_MAC
    // We want the line to be of the colour which is used by Qt to render the
    // border of a 'normal' bordered widget. On Windows and Linux, we can
    // achieve this by setting the frame shape of a QFrame widget, but this
    // doesn't work on OS X, so we need to retrieve that colour from somewhere
    // else...

    // Retrieve, if not already done, the colour used for a 'normal' border
    // Note #1: we use a QTextEdit widget and retrieve the colour of the pixel
    //          which is in the middle of the right border...
    // Note #2: we don't rely on the top border because it may be rendered in a
    //          special way. In the same way, we don't rely on a corner as such
    //          in case it's rendered as a rounded corner...

    static bool firstTime = true;
    static QString borderColorStyle = QString();

    if (firstTime) {
        QTextEdit textEdit;
        QImage image = QImage(textEdit.size(),
                              QImage::Format_ARGB32_Premultiplied);

        textEdit.render(&image);

        QColor borderColor = QColor(image.pixel(image.width()-1,
                                                0.5*image.height()));

        borderColorStyle = QString("QFrame {"
                                   "    border: 1px solid rgb(%1, %2, %3);"
                                   "}").arg(QString::number(borderColor.red()),
                                            QString::number(borderColor.green()),
                                            QString::number(borderColor.blue()));

        firstTime = false;
    }
#endif

    // Create and return our line widget

    QFrame *res = new QFrame(pParent);

#ifdef Q_WS_MAC
    res->setStyleSheet(borderColorStyle);
#else
    res->setFrameShape(QFrame::StyledPanel);
#endif

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
    // Create and return a 'real' horizontal line widget

    return newLineWidget(true, pParent);
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

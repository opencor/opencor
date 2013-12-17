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
// Splash screen window
//==============================================================================

#include "cliutils.h"
//#include "guiutils.h"
#include "splashscreenwindow.h"

//==============================================================================

#include "ui_splashscreenwindow.h"

//==============================================================================

#include <QApplication>
#include <QCoreApplication>
#include <QDesktopWidget>
#include <QElapsedTimer>
#include <QEvent>
#include <QEventLoop>
#include <QImage>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QRect>
#include <QStackedWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <QWindow>

//==============================================================================

#include <qnamespace.h>

//==============================================================================

#ifdef Q_OS_WIN
    #include <qt_windows.h>
#else
    #include <time.h>
#endif

//==============================================================================

namespace OpenCOR {

//==============================================================================

SplashScreenWindow::SplashScreenWindow() :
    QWidget(0, Qt::SplashScreen|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint),
    mGui(new Ui::SplashScreenWindow)
{
    // Set up the GUI

    mGui->setupUi(this);

    QColor borderRgb = borderColor();
    QString borderStyle = "1px solid rgb("+QString::number(borderRgb.red())+", "+QString::number(borderRgb.green())+", "+QString::number(borderRgb.blue())+");";

#ifdef Q_OS_MAC
    setStyleSheet("QWidget#infoWidget {"
                  "    border-top: "+borderStyle+
                  "}");
#else
    setStyleSheet("QLabel#splashScreenImage {"
                  "    border-top: "+borderStyle+
                  "    border-left: "+borderStyle+
                  "    border-right: "+borderStyle+
                  "}"
                  ""
                  "QWidget#infoWidget {"
                  "    border: "+borderStyle+
                  "}");
#endif

    mGui->copyrightValue->setText(Core::copyright());
    mGui->versionValue->setText(Core::shortVersion(qApp));

    // Miscellaneous

    move( QApplication::desktop()->screenGeometry().center()-rect().center());
}

//==============================================================================

SplashScreenWindow::~SplashScreenWindow()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void SplashScreenWindow::finish(QWidget *pWindow)
{
    // Wait for our window to expose itself

    if (pWindow) {
        QWindow *window = pWindow->windowHandle();

        if (window) {
            enum {
                TimeOut = 789,
                ShortDelay = 9
            };

            QElapsedTimer timer;
#ifndef Q_OS_WIN
            struct timespec shortDelaySpec = { ShortDelay/1000, 1000000*(ShortDelay%1000) };
#endif

            timer.start();

            while (!window->isExposed()) {
                int remaining = TimeOut-timer.elapsed();

                if (remaining <= 0)
                    break;

                QCoreApplication::processEvents(QEventLoop::AllEvents, remaining);
                QCoreApplication::sendPostedEvents(0, QEvent::DeferredDelete);

#ifdef Q_OS_WIN
                Sleep(ShortDelay);
#else
                nanosleep(&shortDelaySpec, 0);
#endif
            }
        }
    }

    // Close ourselves with a bit of a delay

    enum {
        FadingDelay = 789
    };

    QTimer::singleShot(FadingDelay, this, SLOT(close()));
}

//==============================================================================

void SplashScreenWindow::mousePressEvent(QMouseEvent *pEvent)
{
    // Hide ourselves

    hide();

    pEvent->accept();
}

//==============================================================================

QColor SplashScreenWindow::borderColor()
{
    // Retrieve the colour used for a 'normal' border
    // Note: we would normally use Core::borderColor(), but it's not yet defined
    //       when we need it here, so we shamelessly copied/pasted the code from
    //       CorePlugin::retrieveBorderColor()...

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

    return QColor(image.pixel(image.width()-1, 0.5*image.height()));
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

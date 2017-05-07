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
// Splash screen window
//==============================================================================

#include "cliutils.h"
#include "guiutils.h"
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
#include <QFont>
#include <QLabel>
#include <QMouseEvent>
#include <QTimer>
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

    setStyleSheet("QLabel#splashScreenImage {"
                  "     background-color: white;"
                  "}"
#ifdef Q_OS_MAC
                  "QWidget#infoWidget {"
                  "    border-top: "+borderStyle+
                  "}"
#else
                  "QLabel#splashScreenImage {"
                  "    border-top: "+borderStyle+
                  "    border-left: "+borderStyle+
                  "    border-right: "+borderStyle+
                  "}"
                  ""
                  "QWidget#infoWidget {"
                  "    border: "+borderStyle+
                  "}"
#endif
                 );

    QFont newFont = mGui->infoWidget->font();

#if defined(Q_OS_WIN)
    newFont.setPointSize(9);
#elif defined(Q_OS_LINUX)
    newFont.setPointSize(8);
#elif defined(Q_OS_MAC)
    newFont.setPointSize(11);
#else
    #error Unsupported platform
#endif

    mGui->copyrightValue->setFont(newFont);
    mGui->versionValue->setFont(newFont);

    mGui->copyrightValue->setText(copyright());
    mGui->versionValue->setText(shortVersion());

    // Adjust the size of our splash screen and then move it to the center of
    // our screen

    adjustSize();
    move(QApplication::desktop()->screenGeometry().center()-rect().center());
}

//==============================================================================

SplashScreenWindow::~SplashScreenWindow()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void SplashScreenWindow::closeAndDeleteAfter(QWidget *pWindow)
{
    // Wait for our window to expose itself

    if (pWindow) {
        QWindow *window = pWindow->windowHandle();

        if (window) {
            enum {
                TimeOut = 1000,
                ShortDelay = 10
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

    QTimer::singleShot(500, this, SLOT(close()));
}

//==============================================================================

void SplashScreenWindow::closeEvent(QCloseEvent *pEvent)
{
    // Accept the event

    pEvent->accept();

    // Ask for ourselves to be deleted later

    deleteLater();
}

//==============================================================================

void SplashScreenWindow::mousePressEvent(QMouseEvent *pEvent)
{
    // Accept the event

    pEvent->accept();

    // Hide ourselves

    hide();
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

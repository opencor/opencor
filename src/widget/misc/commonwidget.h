#ifndef COMMONWIDGET_H
#define COMMONWIDGET_H

#include <QString>
#include <QtGlobal>

class QSettings;
class QSize;
class QWidget;

// The default font family and size below were taken from Qt Creator
#ifdef Q_WS_WIN
static const QString DefaultFontFamily = "Courier";
static const int DefaultFontSize = 10;
#else
#ifdef Q_WS_MAC
static const QString DefaultFontFamily = "Monaco";
static const int DefaultFontSize = 12;
#else
static const QString DefaultFontFamily = "Monospace";
static const int DefaultFontSize = 9;
#endif
#endif

class CommonWidget
{
public:
    explicit CommonWidget(QWidget *pParent);
    explicit CommonWidget(const QString &pName, QWidget *pWidget,
                          QWidget *pParent);

    virtual void retranslateUi();

    virtual void loadSettings(QSettings &pSettings);
    virtual void saveSettings(QSettings &pSettings);

protected:
    QSize defaultSize(const double &pRatio) const;

    virtual void updateActions();

    void drawBorderIfDocked(const bool &pForceDrawing = false,
                            const bool &pTop = true, const bool &pLeft = true,
                            const bool &pBottom = true, const bool &pRight = true);

private:
    QWidget *mParent;
};

#endif

#ifndef COMMONWIDGET_H
#define COMMONWIDGET_H

class QSettings;
class QSize;
class QString;
class QWidget;

class CommonWidget
{
public:
    CommonWidget(QWidget *pParent = 0);

    virtual void retranslateUi();

    virtual void loadSettings(const QSettings &pSettings, const QString &pKey);
    virtual void saveSettings(QSettings &pSettings, const QString &pKey);

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

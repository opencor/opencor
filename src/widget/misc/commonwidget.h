#ifndef COMMONWIDGET_H
#define COMMONWIDGET_H

class QSettings;
class QSize;
class QString;
class QWidget;

class CommonWidget
{
public:
    explicit CommonWidget(QWidget *pParent);
    explicit CommonWidget(const QString &pName, QWidget *pParent);

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

#ifndef COMMONWIDGET_H
#define COMMONWIDGET_H

class QSettings;
class QSize;
class QString;

class CommonWidget
{
public:
    virtual void retranslateUi() = 0;

    virtual void defaultSettings() = 0;
    
    virtual void loadSettings(const QSettings &pSettings, const QString &pKey) = 0;
    virtual void saveSettings(QSettings &pSettings, const QString &pKey) = 0;

protected:
    virtual QSize defaultSize(const double &pRatio) const;
};

#endif

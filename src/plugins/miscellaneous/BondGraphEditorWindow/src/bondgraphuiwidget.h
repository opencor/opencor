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
#pragma once
#include <QWidget>

class QStatusBar;
class QSettings;
class QToolBar;
class QToolButton;

namespace OpenCOR {
namespace BondGraphEditorWindow {

class WidgetBox;
class QAccordion;
class BGElementEditorUIController;

class BondGraphUIWidget : public QWidget
{
    Q_OBJECT

public:
    BondGraphUIWidget(QWidget *parent = nullptr, bool doBackup = false);
    ~BondGraphUIWidget();

    void setupUi();
    void retranslateUi();
    QToolBar *getToolBar() const;

    virtual QSettings &getApplicationSettings() const;
    virtual void saveSettings(QSettings &pSettings) const;
    virtual void readSettings(QSettings &settings);
    virtual void setCentralWidget(QWidget *mainWidget);
    virtual void showStatusMessage(const QString &msg, int timeout = 0);
    virtual QString getCurrentFileName() const;
    virtual QStatusBar *statusBar() const;
    virtual void addControlPane(QString name, QWidget *widget);
    virtual QString iconURI(QString domain, QString element);
    virtual void closeEvent(QCloseEvent *evt);

    virtual void saveFile(const QString &pOldFileName,
                          const QString &pNewFileName, bool &pNeedFeedback);
    virtual void cut();
    virtual void copy();
    virtual void paste();
    virtual void del();
    virtual void undo();
    virtual void redo();

Q_SIGNALS:
    /**
   * @brief Signal that the model was serialized to CellML
   * Path to the cellml file is passed. Useful to connect to a CellML editor
   * 
   * @param path 
   */
    void cellMLFileGenerated(QString path);

protected:
    virtual void showEvent(QShowEvent *event);

private:
    WidgetBox *m_widgetBox;
    QAccordion *m_controls;
    QWidget *m_annotation;
    QWidget *m_canvasParent;
    QStatusBar *m_statusBar;
    QToolBar *m_toolBar;
    BGElementEditorUIController *uiController;
    QString m_lastPath;
    QString m_currentFileName;
    bool m_controlsVisible = true;
    bool m_widgetBoxVisible = true;
    bool m_initialShowEvent = true;
    bool m_doBackup;
};
} // namespace BondGraphEditorWindow
} // namespace OpenCOR
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
// PMR support preferences widget
//==============================================================================

#include "pmrsupportpreferenceswidget.h"

//==============================================================================

#include "ui_pmrsupportpreferenceswidget.h"

//==============================================================================

#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

static const auto StagingInstance  = QStringLiteral("https://staging.physiomeproject.org");
static const auto TeachingInstance = QStringLiteral("https://teaching.physiomeproject.org");

//==============================================================================

PmrSupportPreferencesWidget::PmrSupportPreferencesWidget(QWidget *pParent) :
    Preferences::PreferencesWidget(PluginName, pParent),
    mGui(new Ui::PmrSupportPreferencesWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    connect(mGui->pmrUrlValue, &QComboBox::currentTextChanged,
            this, &PmrSupportPreferencesWidget::pmrUrlValueCurrentTextChanged);

    mGui->pmrUrlValue->addItems(QStringList() << SettingsPreferencesPmrUrlDefault
                                              << StagingInstance
                                              << TeachingInstance);

#ifdef Q_OS_MAC
    mGui->nameValue->setAttribute(Qt::WA_MacShowFocusRect, false);
    mGui->emailValue->setAttribute(Qt::WA_MacShowFocusRect, false);
#endif

    mPmrUrl = mSettings->value(SettingsPreferencesPmrUrl, SettingsPreferencesPmrUrlDefault).toString();
    mName = mSettings->value(SettingsPreferencesName).toString();
    mEmail = mSettings->value(SettingsPreferencesEmail).toString();

    mGui->pmrUrlValue->setCurrentText(mPmrUrl);
    mGui->nameValue->setText(mName);
    mGui->emailValue->setText(mEmail);

    setFocusProxy(mGui->pmrUrlValue);
}

//==============================================================================

PmrSupportPreferencesWidget::~PmrSupportPreferencesWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

bool PmrSupportPreferencesWidget::preferencesChanged() const
{
    // Return whether our preferences have changed

    return    mGui->pmrUrlValue->currentText().compare(mPmrUrl)
           || mGui->nameValue->text().compare(mName)
           || mGui->emailValue->text().compare(mEmail);
}

//==============================================================================

void PmrSupportPreferencesWidget::resetPreferences()
{
    // Reset our preferences

    mGui->pmrUrlValue->setCurrentText(SettingsPreferencesPmrUrlDefault);
    mGui->nameValue->setText(QString());
    mGui->emailValue->setText(QString());
}

//==============================================================================

void PmrSupportPreferencesWidget::savePreferences()
{
    // Save our preferences

    mSettings->setValue(SettingsPreferencesPmrUrl, mGui->pmrUrlValue->currentText());
    mSettings->setValue(SettingsPreferencesName, mGui->nameValue->text());
    mSettings->setValue(SettingsPreferencesEmail, mGui->emailValue->text());
}

//==============================================================================

void PmrSupportPreferencesWidget::pmrUrlValueCurrentTextChanged(const QString &pCurrentText)
{
    // Update our PMR URL note based on the PMR URL that is currently selected

    if (!pCurrentText.compare(SettingsPreferencesPmrUrlDefault))
        mGui->noteValue->setText(tr("the primary site is selected. Everything on this site is permanent and persistent. It is always up and always stable."));
    else if (!pCurrentText.compare(StagingInstance))
        mGui->noteValue->setText(tr("the staging site is selected. It is used for public testing/preview of PMR developments. Data on this site is wiped periodically whenever a new public testing/preview of the PMR software suite is released for the required testing exercise."));
    else if (!pCurrentText.compare(TeachingInstance))
        mGui->noteValue->setText(tr("the teaching site is selected. The functionality of this site should match the primary site, but without the data persistency guarantees. While data on this site is also not permanent, any wipes to data stored will be announced on the cellml-discussion mailing list."));
    else
        mGui->noteValue->setText(QString());

    mGui->noteLabel->setVisible(!mGui->noteValue->text().isEmpty());
}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

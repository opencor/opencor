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
// Core settings
//==============================================================================

#ifndef CORESETTINGS_H
#define CORESETTINGS_H

//==============================================================================

#include <QString>

//==============================================================================

namespace OpenCOR {

//==============================================================================

static const auto SettingsGlobal          = QStringLiteral("Global");
static const auto SettingsLocale          = QStringLiteral("Locale");
static const auto SettingsActiveDirectory = QStringLiteral("ActiveDirectory");

//==============================================================================

static const auto SettingsBaseColor      = QStringLiteral("BaseColor");
static const auto SettingsBorderColor    = QStringLiteral("BorderColor");
static const auto SettingsHighlightColor = QStringLiteral("HighlightColor");
static const auto SettingsLinkColor      = QStringLiteral("LinkColor");
static const auto SettingsShadowColor    = QStringLiteral("Shadow");
static const auto SettingsWindowColor    = QStringLiteral("WindowColor");

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

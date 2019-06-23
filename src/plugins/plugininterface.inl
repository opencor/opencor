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
// Plugin interface
//==============================================================================

#ifdef INTERFACE_DEFINITION
    #define VIRTUAL virtual
    #define PURE_OR_OVERRIDE = 0
#else
    #define VIRTUAL
    #define PURE_OR_OVERRIDE override
#endif

    // Note: make sure to update pluginInterfaceVersion() whenever you update
    //       this interface...

    VIRTUAL bool definesPluginInterfaces() PURE_OR_OVERRIDE;
    VIRTUAL bool pluginInterfacesOk(const QString &pFileName,
                                    QObject *pInstance) PURE_OR_OVERRIDE;

    VIRTUAL void initializePlugin() PURE_OR_OVERRIDE;
    VIRTUAL void finalizePlugin() PURE_OR_OVERRIDE;

    VIRTUAL void pluginsInitialized(const Plugins &pLoadedPlugins) PURE_OR_OVERRIDE;

    VIRTUAL void loadSettings(QSettings &pSettings) PURE_OR_OVERRIDE;
    VIRTUAL void saveSettings(QSettings &pSettings) const PURE_OR_OVERRIDE;

    VIRTUAL void handleUrl(const QUrl &pUrl) PURE_OR_OVERRIDE;

#undef VIRTUAL
#undef PURE_OR_OVERRIDE

//==============================================================================
// End of file
//==============================================================================

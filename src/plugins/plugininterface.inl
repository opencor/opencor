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
    #define PURE = 0
#else
    #define PURE
#endif

    // Note: make sure to update pluginInterfaceVersion() whenever you update
    //       this interface...

    virtual bool definesPluginInterfaces() PURE;
    virtual bool pluginInterfacesOk(const QString &pFileName,
                                    QObject *pInstance) PURE;

    virtual void initializePlugin() PURE;
    virtual void finalizePlugin() PURE;

    virtual void pluginsInitialized(const Plugins &pLoadedPlugins) PURE;

    virtual void loadSettings(QSettings *pSettings) PURE;
    virtual void saveSettings(QSettings *pSettings) const PURE;

    virtual void handleUrl(const QUrl &pUrl) PURE;

#undef PURE

//==============================================================================
// End of file
//==============================================================================

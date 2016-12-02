/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// File handling interface
//==============================================================================

#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    // Note: changes to this interface must result in pluginVersion() being
    //       updated (see [OpenCOR]/src/plugins/pluginInfo.cpp)...

    virtual bool isIndirectRemoteFile(const QString &pFileName) PURE;
    // Note: this method is called by the central widget to determine whether it
    //       should be showing its busy widget before calling viewWidget() for
    //       a given file name. Indeed, the central widget will automatically
    //       show its busy widget if a file is a remote one, but in the case of
    //       a local SED-ML file or a local COMBINE archive, it cannot tell
    //       whether it should show its busy widget since it depends on the
    //       referenced model, i.e. whether it references a local model or a
    //       remote one. So, since the central widget has no knowledge of file
    //       types, we need to ask the plugin to tell us whether a given file is
    //       an indirect remote file...

    virtual bool saveFile(const QString &pOldFileName,
                          const QString &pNewFileName,
                          bool &pNeedFeedback) PURE;

    virtual void fileOpened(const QString &pFileName) PURE;
    virtual void filePermissionsChanged(const QString &pFileName) PURE;
    virtual void fileModified(const QString &pFileName) PURE;
    virtual void fileReloaded(const QString &pFileName,
                              const bool &pFileChanged) PURE;
    virtual void fileRenamed(const QString &pOldFileName,
                             const QString &pNewFileName) PURE;
    virtual void fileClosed(const QString &pFileName) PURE;

#undef PURE

//==============================================================================
// End of file
//==============================================================================

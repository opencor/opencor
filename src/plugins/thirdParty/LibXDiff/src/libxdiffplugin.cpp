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
// LibXDiff plugin
//==============================================================================

#include "libxdiffplugin.h"

//==============================================================================

#include "xdiff.h"

//==============================================================================

namespace OpenCOR {
namespace LibXDiff {

//==============================================================================

PLUGININFO_FUNC LibXDiffPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to access <a href=\"http://www.xmailserver.org/xdiff-lib.html\">LibXDiff</a>."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder <a href=\"http://www.xmailserver.org/xdiff-lib.html\">LibXDiff</a>."));

    return new PluginInfo(PluginInfo::ThirdParty, false, false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

static void *mallocWrapper(void *pData, unsigned int pSize)
{
    Q_UNUSED(pData);

    // Allocate some memory

    return malloc(pSize);
}

//==============================================================================

static void freeWrapper(void *pData, void *pPointer)
{
    Q_UNUSED(pData);

    // Free the given memory

    free(pPointer);
}

//==============================================================================

static void *reallocWrapper(void *pData, void *pPointer, unsigned int pSize)
{
    Q_UNUSED(pData);

    // Reallocate some memory

    return realloc(pPointer, pSize);
}

//==============================================================================

LibXDiffPlugin::LibXDiffPlugin()
{
    // Set our memory allocator

    memallocator_t memoryAllocator = { 0, mallocWrapper, freeWrapper, reallocWrapper };

    xdl_set_allocator(&memoryAllocator);
}

//==============================================================================

}   // namespace LibXDiff
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

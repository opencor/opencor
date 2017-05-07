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

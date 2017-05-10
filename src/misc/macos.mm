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
// macOS utilities
//==============================================================================

#include "macos.h"

//==============================================================================

#import <AppKit/NSWindow.h>

//==============================================================================

namespace OpenCOR {

//==============================================================================

void removeMacosSpecificMenuItems()
{
    // Remove (disable) the "Start Dictation..." and "Emoji & Symbols" menu
    // items from the "Edit" menu

    [[NSUserDefaults standardUserDefaults] setBool:YES forKey:@"NSDisabledDictationMenuItem"];
    [[NSUserDefaults standardUserDefaults] setBool:YES forKey:@"NSDisabledCharacterPaletteMenuItem"];

#ifdef AVAILABLE_MAC_OS_X_VERSION_10_12_AND_LATER
    // Remove (don't allow) the "Show Tab Bar" menu item from the "View" menu,
    // if supported

    if ([NSWindow respondsToSelector:@selector(allowsAutomaticWindowTabbing)])
        NSWindow.allowsAutomaticWindowTabbing = NO;
#endif

    // Remove (don't have) the "Enter Full Screen" menu item from the "View"
    // menu

    [[NSUserDefaults standardUserDefaults] setBool:NO forKey:@"NSFullScreenMenuItemEverywhere"];
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
// SampleTools plugin
//==============================================================================

#include "sampletoolsplugin.h"
#include "sampleutils.h"

//==============================================================================

#include <iostream>

//==============================================================================

namespace OpenCOR {
namespace SampleTools {

//==============================================================================

PLUGININFO_FUNC SampleToolsPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin that provides a simple addition tool."));
    descriptions.insert("fr", QString::fromUtf8("une extension qui fournit un simple outil d'addition."));

    return new PluginInfo(PluginInfo::Sample, true,
                          QStringList() << "Sample",
                          descriptions);
}

//==============================================================================
// CLI interface
//==============================================================================

int SampleToolsPlugin::execute(const QString &pCommand,
                               const QStringList &pArguments)
{
    // Run the given CLI command

    if (!pCommand.compare("help")) {
        // Display the commands that we support

        runHelpCommand();

        return 0;
    } else if (!pCommand.compare("add")) {
        // Add some numbers

        return runAddCommand(pArguments);
    } else {
        // Not a CLI command that we support, so...

        runHelpCommand();

        return -1;
    }
}

//==============================================================================
// Plugin specific
//==============================================================================

void SampleToolsPlugin::runHelpCommand()
{
    // Output the commands we support

    std::cout << "Commands supported by SampleTools:" << std::endl;
    std::cout << " * Display the commands supported by SampleTools:" << std::endl;
    std::cout << "      help" << std::endl;
    std::cout << " * Add two numbers:" << std::endl;
    std::cout << "      add <nb1> <nb2>" << std::endl;
}

//==============================================================================

int SampleToolsPlugin::runAddCommand(const QStringList &pArguments)
{
    // Make sure that we have the correct number of arguments

    if (pArguments.count() != 2) {
        runHelpCommand();

        return -1;
    }

    // Make sure that the two arguments are valid numbers

    bool ok;

    double nb1 = pArguments.first().toDouble(&ok);

    if (!ok) {
        std::cout << "Sorry, but " << qPrintable(pArguments.first()) << " is not a valid number." << std::endl;

        return -1;
    }

    double nb2 = pArguments.last().toDouble(&ok);

    if (!ok) {
        std::cout << "Sorry, but " << qPrintable(pArguments.last()) << " is not a valid number." << std::endl;

        return -1;
    }

    // Add the two numbers and output the result

    std::cout << qPrintable(pArguments.first()) << " + " << qPrintable(pArguments.last()) << " = " << Sample::add(nb1, nb2) << std::endl;

    return 0;
}

//==============================================================================

}   // namespace SampleTools
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

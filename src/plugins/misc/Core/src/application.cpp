//==============================================================================
// Application class
//==============================================================================

#include "application.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

Application::Application(int pArgc, char *pArgv[]) :
    QtSingleApplication(pArgc, pArgv)
{
}

//==============================================================================

void * Application::singleton(const QString &pClassName,
                              void *pDefaultClassInstance)
{
    // Check whether a single exists for the requested class

    if (mSingletons.value(pClassName)) {
        // Yes, there is one, so just return it

        return mSingletons.value(pClassName);
    } else {
        // No, there isn't one, so use the default class instance, keep track of
        // it and return it

        mSingletons.insert(pClassName, pDefaultClassInstance);

        return pDefaultClassInstance;
    }
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

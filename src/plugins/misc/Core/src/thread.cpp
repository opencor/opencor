//==============================================================================
// Thread
//==============================================================================

#include "thread.h"

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

void Thread::sleep(const size_t &pDuration)
{
    // Sleep for a few seconds

    QThread::sleep(pDuration);
}

//==============================================================================

void Thread::msleep(const size_t &pDuration)
{
    // Sleep for a few milliseconds

    QThread::msleep(pDuration);
}

//==============================================================================

void Thread::usleep(const size_t &pDuration)
{
    // Sleep for a few microseconds
    // Note: this may not be reliable on some platforms...

    QThread::usleep(pDuration);
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

#if defined(Q_OS_WIN)
    #pragma warning(default: 4966)
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    #pragma GCC diagnostic warning "-Wunused-variable"
#else
    #error Unsupported platform
#endif

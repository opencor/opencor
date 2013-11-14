#if defined(Q_OS_WIN)
    #pragma warning(disable: 4966)
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    #pragma GCC diagnostic ignored "-Wunused-variable"
#else
    #error Unsupported platform
#endif

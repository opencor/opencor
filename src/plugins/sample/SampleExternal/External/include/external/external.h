//==============================================================================

// An example header file

//==============================================================================

#pragma once

//==============================================================================

#ifdef _WIN32
    #ifdef EXTERNAL_LIBRARY
        #define EXTERNAL_EXPORT __declspec(dllexport)
    #else
        #define EXTERNAL_EXPORT __declspec(dllimport)
    #endif
#else
    #define EXTERNAL_EXPORT
#endif

namespace External {

//==============================================================================

// A dummy class

class EXTERNAL_EXPORT Example {

    Example() ;

};

//==============================================================================

} ;

//==============================================================================

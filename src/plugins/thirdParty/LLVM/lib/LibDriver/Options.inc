/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Option Parsing Definitions                                                 *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

/////////
// Prefixes

#ifdef PREFIX
#define COMMA ,
PREFIX(prefix_0, {nullptr})
PREFIX(prefix_1, {"/" COMMA "-" COMMA "-?" COMMA nullptr})
#undef COMMA
#endif // PREFIX

/////////
// Groups

#ifdef OPTION

//////////
// Options

OPTION(prefix_0, "<input>", INPUT, Input, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr)
OPTION(prefix_0, "<unknown>", UNKNOWN, Unknown, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr)
OPTION(prefix_1, "ignore:", ignore, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr)
OPTION(prefix_1, "libpath:", libpath, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Object file search path", nullptr)
OPTION(prefix_1, "llvmlibthin", llvmlibthin, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr)
OPTION(prefix_1, "machine:", machine, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr)
OPTION(prefix_1, "nologo", nologo, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr)
OPTION(prefix_1, "out:", out, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Path to file to write output", nullptr)
#endif // OPTION

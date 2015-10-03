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
PREFIX(prefix_0, {0})
PREFIX(prefix_1, {"/" COMMA "-" COMMA "-?" COMMA 0})
#undef COMMA
#endif

/////////
// Groups

#ifdef OPTION

//////////
// Options

OPTION(prefix_0, "<input>", INPUT, Input, INVALID, INVALID, 0, 0, 0, 0, 0)
OPTION(prefix_0, "<unknown>", UNKNOWN, Unknown, INVALID, INVALID, 0, 0, 0, 0, 0)
OPTION(prefix_1, "ignore:", ignore, Joined, INVALID, INVALID, 0, 0, 0, 0, 0)
OPTION(prefix_1, "libpath:", libpath, Joined, INVALID, INVALID, 0, 0, 0,
       "Object file search path", 0)
OPTION(prefix_1, "machine:", machine, Joined, INVALID, INVALID, 0, 0, 0, 0, 0)
OPTION(prefix_1, "nologo", nologo, Flag, INVALID, INVALID, 0, 0, 0, 0, 0)
OPTION(prefix_1, "out:", out, Joined, INVALID, INVALID, 0, 0, 0,
       "Path to file to write output", 0)
#endif

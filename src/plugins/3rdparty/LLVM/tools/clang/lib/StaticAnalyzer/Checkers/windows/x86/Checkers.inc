
#ifdef GET_GROUPS
#endif // GET_GROUPS


#ifdef GET_PACKAGES
PACKAGE("core", -1, false)
PACKAGE("core.builtin", -1, false)
PACKAGE("core.uninitialized", -1, false)
PACKAGE("cplusplus", -1, false)
PACKAGE("deadcode", -1, false)
PACKAGE("debug", -1, false)
PACKAGE("experimental", -1, false)
PACKAGE("experimental.core", -1, true)
PACKAGE("experimental.cplusplus", -1, true)
PACKAGE("experimental.deadcode", -1, true)
PACKAGE("experimental.osx", -1, true)
PACKAGE("experimental.osx.cocoa", -1, true)
PACKAGE("experimental.security", -1, true)
PACKAGE("experimental.security.taint", -1, true)
PACKAGE("experimental.unix", -1, true)
PACKAGE("experimental.unix.cstring", -1, true)
PACKAGE("llvm", -1, false)
PACKAGE("osx", -1, false)
PACKAGE("osx.cocoa", -1, false)
PACKAGE("osx.coreFoundation", -1, false)
PACKAGE("osx.coreFoundation.containers", -1, false)
PACKAGE("security", -1, false)
PACKAGE("security.insecureAPI", -1, false)
PACKAGE("unix", -1, false)
PACKAGE("unix.cstring", -1, true)
#endif // GET_PACKAGES


#ifdef GET_CHECKERS
CHECKER("core.AdjustedReturnValue", AdjustedReturnValueChecker, AdjustedReturnValueChecker.cpp, "Check to see if the return value of a function call is different than the caller expects (e.g., from calls through function pointers)", -1, false)
CHECKER("debug.Stats", AnalyzerStatsChecker, AnalyzerStatsChecker.cpp, "Emit warnings with analyzer statistics", -1, false)
CHECKER("experimental.security.ArrayBound", ArrayBoundChecker, ArrayBoundChecker.cpp, "Warn about buffer overflows (older checker)", -1, true)
CHECKER("experimental.security.ArrayBoundV2", ArrayBoundCheckerV2, ArrayBoundCheckerV2.cpp, "Warn about buffer overflows (newer checker)", -1, true)
CHECKER("core.AttributeNonNull", AttrNonNullChecker, AttrNonNullChecker.cpp, "Check for null pointers passed as arguments to a function whose arguments are marked with the 'nonnull' attribute", -1, false)
CHECKER("experimental.core.BoolAssignment", BoolAssignmentChecker, BoolAssignmentChecker.cpp, "Warn about assigning non-{0,1} values to Boolean variables", -1, true)
CHECKER("core.builtin.BuiltinFunctions", BuiltinFunctionChecker, BuiltinFunctionChecker.cpp, "Evaluate compiler builtin functions (e.g., alloca())", -1, false)
CHECKER("osx.coreFoundation.CFError", CFErrorChecker, NSErrorChecker.cpp, "Check usage of CFErrorRef* parameters", -1, false)
CHECKER("debug.DumpCFG", CFGDumper, DebugCheckers.cpp, "Display Control-Flow Graphs", -1, false)
CHECKER("debug.ViewCFG", CFGViewer, DebugCheckers.cpp, "View Control-Flow Graphs using GraphViz", -1, false)
CHECKER("osx.coreFoundation.CFNumber", CFNumberCreateChecker, BasicObjCFoundationChecks.cpp, "Check for proper uses of CFNumberCreate", -1, false)
CHECKER("osx.coreFoundation.CFRetainRelease", CFRetainReleaseChecker, BasicObjCFoundationChecks.cpp, "Check for null arguments to CFRetain/CFRelease", -1, false)
CHECKER("experimental.unix.cstring.BufferOverlap", CStringBufferOverlap, CStringChecker.cpp, "Checks for overlap in two buffer arguments", -1, true)
CHECKER("experimental.unix.cstring.NotNullTerminated", CStringNotNullTerm, CStringChecker.cpp, "Check for arguments which are not null-terminating strings", -1, true)
CHECKER("unix.cstring.NullArg", CStringNullArg, CStringChecker.cpp, "Check for null pointers being passed as arguments to C string functions", -1, true)
CHECKER("experimental.unix.cstring.OutOfBounds", CStringOutOfBounds, CStringChecker.cpp, "Check for out-of-bounds access in string functions", -1, true)
CHECKER("unix.cstring.BadSizeArg", CStringSyntaxChecker, CStringSyntaxChecker.cpp, "Check the size argument passed into C string functions for common erroneous patterns", -1, true)
CHECKER("core.CallAndMessage", CallAndMessageChecker, CallAndMessageChecker.cpp, "Check for logical errors for function calls and Objective-C message expressions (e.g., uninitialized arguments, null function pointers)", -1, false)
CHECKER("debug.DumpCallGraph", CallGraphDumper, DebugCheckers.cpp, "Display Call Graph", -1, false)
CHECKER("debug.ViewCallGraph", CallGraphViewer, DebugCheckers.cpp, "View Call Graph using GraphViz", -1, false)
CHECKER("experimental.core.CastSize", CastSizeChecker, CastSizeChecker.cpp, "Check when casting a malloc'ed type T, whether the size is a multiple of the size of T", -1, true)
CHECKER("experimental.core.CastToStruct", CastToStructChecker, CastToStructChecker.cpp, "Check for cast from non-struct pointer to struct pointer", -1, true)
CHECKER("experimental.unix.Chroot", ChrootChecker, ChrootChecker.cpp, "Check improper use of chroot", -1, true)
CHECKER("osx.cocoa.ClassRelease", ClassReleaseChecker, BasicObjCFoundationChecks.cpp, "Check for sending 'retain', 'release', or 'autorelease' directly to a Class", -1, false)
CHECKER("deadcode.DeadStores", DeadStoresChecker, DeadStoresChecker.cpp, "Check for values stored to variables that are never read afterwards", -1, false)
CHECKER("core.NullDereference", DereferenceChecker, DereferenceChecker.cpp, "Check for dereferences of null pointers", -1, false)
CHECKER("core.DivideZero", DivZeroChecker, DivZeroChecker.cpp, "Check for division by zero", -1, false)
CHECKER("debug.DumpDominators", DominatorsTreeDumper, DebugCheckers.cpp, "Print the dominance tree for a given CFG", -1, false)
CHECKER("experimental.core.FixedAddr", FixedAddressChecker, FixedAddressChecker.cpp, "Check for assignment of a fixed address to a pointer", -1, true)
CHECKER("security.FloatLoopCounter", FloatLoopCounter, CheckSecuritySyntaxOnly.cpp, "Warn on using a floating point value as a loop counter (CERT: FLP30-C, FLP30-CPP)", -1, false)
CHECKER("experimental.security.taint.TaintPropagation", GenericTaintChecker, GenericTaintChecker.cpp, "Generate taint information used by other checkers", -1, true)
CHECKER("experimental.deadcode.IdempotentOperations", IdempotentOperationChecker, IdempotentOperationChecker.cpp, "Warn about idempotent operations", -1, true)
CHECKER("experimental.cplusplus.Iterators", IteratorsChecker, IteratorsChecker.cpp, "Check improper uses of STL vector iterators", -1, true)
CHECKER("llvm.Conventions", LLVMConventionsChecker, LLVMConventionsChecker.cpp, "Check code for LLVM codebase conventions", -1, false)
CHECKER("debug.DumpLiveVars", LiveVariablesDumper, DebugCheckers.cpp, "Print results of live variable analysis", -1, false)
CHECKER("osx.SecKeychainAPI", MacOSKeychainAPIChecker, MacOSKeychainAPIChecker.cpp, "Check for proper uses of Secure Keychain APIs", -1, false)
CHECKER("osx.API", MacOSXAPIChecker, MacOSXAPIChecker.cpp, "Check for proper uses of various Mac OS X APIs", -1, false)
CHECKER("experimental.unix.MallocWithAnnotations", MallocOptimistic, MallocChecker.cpp, "Check for memory leaks, double free, and use-after-free problems. Assumes that all user-defined functions which might free a pointer are annotated.", -1, true)
CHECKER("experimental.security.MallocOverflow", MallocOverflowSecurityChecker, MallocOverflowSecurityChecker.cpp, "Check for overflows in the arguments to malloc()", -1, true)
CHECKER("unix.Malloc", MallocPessimistic, MallocChecker.cpp, "Check for memory leaks, double free, and use-after-free problems.", -1, false)
CHECKER("experimental.unix.MallocSizeof", MallocSizeofChecker, MallocSizeofChecker.cpp, "Check for dubious malloc arguments involving sizeof", -1, true)
CHECKER("osx.cocoa.NSAutoreleasePool", NSAutoreleasePoolChecker, NSAutoreleasePoolChecker.cpp, "Warn for suboptimal uses of NSAutoreleasePool in Objective-C GC mode", -1, false)
CHECKER("osx.cocoa.NSError", NSErrorChecker, NSErrorChecker.cpp, "Check usage of NSError** parameters", -1, false)
CHECKER("osx.cocoa.NilArg", NilArgChecker, BasicObjCFoundationChecks.cpp, "Check for prohibited nil arguments to ObjC method calls", -1, false)
CHECKER("core.builtin.NoReturnFunctions", NoReturnFunctionChecker, NoReturnFunctionChecker.cpp, "Evaluate \"panic\" functions that are known to not return to the caller", -1, false)
CHECKER("osx.AtomicCAS", OSAtomicChecker, OSAtomicChecker.cpp, "Evaluate calls to OSAtomic functions", -1, false)
CHECKER("osx.cocoa.AtSync", ObjCAtSyncChecker, ObjCAtSyncChecker.cpp, "Check for null pointers used as mutexes for @synchronized", -1, false)
CHECKER("osx.coreFoundation.containers.PointerSizedValues", ObjCContainersASTChecker, ObjCContainersASTChecker.cpp, "Warns if 'CFArray', 'CFDictionary', 'CFSet' are created with non-pointer-size values", -1, false)
CHECKER("osx.coreFoundation.containers.OutOfBounds", ObjCContainersChecker, ObjCContainersChecker.cpp, "Checks for index out-of-bounds when using 'CFArray' API", -1, false)
CHECKER("experimental.osx.cocoa.Dealloc", ObjCDeallocChecker, CheckObjCDealloc.cpp, "Warn about Objective-C classes that lack a correct implementation of -dealloc", -1, true)
CHECKER("osx.cocoa.IncompatibleMethodTypes", ObjCMethSigsChecker, CheckObjCInstMethSignature.cpp, "Warn about Objective-C method signatures with type incompatibilities", -1, false)
CHECKER("osx.cocoa.SelfInit", ObjCSelfInitChecker, ObjCSelfInitChecker.cpp, "Check that 'self' is properly initialized inside an initializer method", -1, false)
CHECKER("osx.cocoa.UnusedIvars", ObjCUnusedIvarsChecker, ObjCUnusedIVarsChecker.cpp, "Warn about private ivars that are never used", -1, false)
CHECKER("experimental.core.PointerArithm", PointerArithChecker, PointerArithChecker, "Check for pointer arithmetic on locations other than array elements", -1, true)
CHECKER("experimental.core.PointerSub", PointerSubChecker, PointerSubChecker, "Check for pointer subtractions on two pointers pointing to different memory chunks", -1, true)
CHECKER("experimental.unix.PthreadLock", PthreadLockChecker, PthreadLockChecker.cpp, "Simple lock -> unlock checker", -1, true)
CHECKER("osx.cocoa.RetainCount", RetainCountChecker, RetainCountChecker.cpp, "Check for leaks and improper reference count management", -1, false)
CHECKER("experimental.security.ReturnPtrRange", ReturnPointerRangeChecker, ReturnPointerRangeChecker.cpp, "Check for an out-of-bound pointer being returned to callers", -1, true)
CHECKER("core.uninitialized.UndefReturn", ReturnUndefChecker, ReturnUndefChecker.cpp, "Check for uninitialized values being returned to the caller", -1, false)
CHECKER("experimental.core.SizeofPtr", SizeofPointerChecker, CheckSizeofPointer.cpp, "Warn about unintended use of sizeof() on pointer expressions", -1, true)
CHECKER("core.StackAddressEscape", StackAddrEscapeChecker, StackAddrEscapeChecker.cpp, "Check that addresses to stack memory do not escape the function", -1, false)
CHECKER("experimental.unix.Stream", StreamChecker, StreamChecker.cpp, "Check stream handling functions", -1, true)
CHECKER("debug.TaintTest", TaintTesterChecker, TaintTesterChecker.cpp, "Mark tainted symbols as such.", -1, false)
CHECKER("security.insecureAPI.UncheckedReturn", UncheckedReturn, CheckSecuritySyntaxOnly.cpp, "Warn on uses of functions whose return values must be always checked", -1, false)
CHECKER("core.uninitialized.Branch", UndefBranchChecker, UndefBranchChecker.cpp, "Check for uninitialized values used as branch conditions", -1, false)
CHECKER("core.uninitialized.CapturedBlockVariable", UndefCapturedBlockVarChecker, UndefCapturedBlockVarChecker.cpp, "Check for blocks that capture uninitialized values", -1, false)
CHECKER("core.UndefinedBinaryOperatorResult", UndefResultChecker, UndefResultChecker.cpp, "Check for undefined results of binary operators", -1, false)
CHECKER("core.uninitialized.ArraySubscript", UndefinedArraySubscriptChecker, UndefinedArraySubscriptChecker.cpp, "Check for uninitialized values used as array subscripts", -1, false)
CHECKER("core.uninitialized.Assign", UndefinedAssignmentChecker, UndefinedAssignmentChecker.cpp, "Check for assigning uninitialized values", -1, false)
CHECKER("unix.API", UnixAPIChecker, UnixAPIChecker.cpp, "Check calls to various UNIX/Posix functions", -1, false)
CHECKER("experimental.deadcode.UnreachableCode", UnreachableCodeChecker, UnreachableCodeChecker.cpp, "Check unreachable code", -1, true)
CHECKER("core.VLASize", VLASizeChecker, VLASizeChecker.cpp, "Check for declarations of VLA of undefined or zero size", -1, false)
CHECKER("osx.cocoa.VariadicMethodTypes", VariadicMethodTypeChecker, BasicObjCFoundationChecks.cpp, "Check for passing non-Objective-C types to variadic methods that expect only Objective-C types", -1, false)
CHECKER("experimental.cplusplus.VirtualCall", VirtualCallChecker, VirtualCallChecker.cpp, "Check virtual function calls during construction or destruction", -1, true)
CHECKER("security.insecureAPI.getpw", getpw, CheckSecuritySyntaxOnly.cpp, "Warn on uses of the 'getpw' function", -1, false)
CHECKER("security.insecureAPI.gets", gets, CheckSecuritySyntaxOnly.cpp, "Warn on uses of the 'gets' function", -1, false)
CHECKER("security.insecureAPI.mkstemp", mkstemp, CheckSecuritySyntaxOnly.cpp, "Warn when 'mkstemp' is passed fewer than 6 X's in the format string", -1, false)
CHECKER("security.insecureAPI.mktemp", mktemp, CheckSecuritySyntaxOnly.cpp, "Warn on uses of the 'mktemp' function", -1, false)
CHECKER("security.insecureAPI.rand", rand, CheckSecuritySyntaxOnly.cpp, "Warn on uses of the 'rand', 'random', and related functions", -1, false)
CHECKER("security.insecureAPI.strcpy", strcpy, CheckSecuritySyntaxOnly.cpp, "Warn on uses of the 'strcpy' and 'strcat' functions", -1, false)
CHECKER("security.insecureAPI.vfork", vfork, CheckSecuritySyntaxOnly.cpp, "Warn on uses of the 'vfork' function", -1, false)
#endif // GET_CHECKERS


#ifdef GET_MEMBER_ARRAYS
static const short SubPackageArray0[] = { 9, 12, 5, 3, 1, 2, 8, 4, 7, 6, -1 };
static const short CheckerArray1[] = { 0, -1 };
static const short CheckerArray2[] = { 4, -1 };
static const short CheckerArray3[] = { 17, -1 };
static const short CheckerArray4[] = { 26, -1 };
static const short CheckerArray5[] = { 25, -1 };
static const short CheckerArray6[] = { 60, -1 };
static const short CheckerArray7[] = { 66, -1 };
static const short CheckerArray8[] = { 71, -1 };
static const short SubPackageArray9[] = { 11, 10, -1 };
static const short CheckerArray10[] = { 6, -1 };
static const short CheckerArray11[] = { 44, -1 };
static const short SubPackageArray12[] = { 13, 14, 15, 16, 17, -1 };
static const short CheckerArray13[] = { 67, -1 };
static const short CheckerArray14[] = { 68, -1 };
static const short CheckerArray15[] = { 64, -1 };
static const short CheckerArray16[] = { 65, -1 };
static const short CheckerArray17[] = { 58, -1 };
static const short SubPackageArray19[] = { 20, -1 };
static const short CheckerArray20[] = { 24, -1 };
static const short SubPackageArray21[] = { 24, 25, 28, 22, 29, 23, 26, 27, -1 };
static const short CheckerArray22[] = { 8, -1 };
static const short CheckerArray23[] = { 18, -1 };
static const short CheckerArray24[] = { 27, -1 };
static const short CheckerArray25[] = { 34, -1 };
static const short CheckerArray26[] = { 1, -1 };
static const short CheckerArray27[] = { 62, -1 };
static const short CheckerArray28[] = { 9, -1 };
static const short CheckerArray29[] = { 19, -1 };
static const short SubPackageArray30[] = { 31, 39, 42, 48, 55, 45, -1 };
static const short SubPackageArray31[] = { 32, 33, 34, 35, 36, 37, 38, -1 };
static const short CheckerArray32[] = { 5, -1 };
static const short CheckerArray33[] = { 20, -1 };
static const short CheckerArray34[] = { 21, -1 };
static const short CheckerArray35[] = { 28, -1 };
static const short CheckerArray36[] = { 53, -1 };
static const short CheckerArray37[] = { 54, -1 };
static const short CheckerArray38[] = { 59, -1 };
static const short SubPackageArray39[] = { 40, 41, -1 };
static const short CheckerArray40[] = { 32, -1 };
static const short CheckerArray41[] = { 73, -1 };
static const short SubPackageArray42[] = { 43, 44, -1 };
static const short CheckerArray43[] = { 31, -1 };
static const short CheckerArray44[] = { 70, -1 };
static const short SubPackageArray45[] = { 46, -1 };
static const short SubPackageArray46[] = { 47, -1 };
static const short CheckerArray47[] = { 49, -1 };
static const short SubPackageArray48[] = { 53, 49, 50, 52, 51, -1 };
static const short CheckerArray49[] = { 2, -1 };
static const short CheckerArray50[] = { 3, -1 };
static const short CheckerArray51[] = { 38, -1 };
static const short CheckerArray52[] = { 57, -1 };
static const short SubPackageArray53[] = { 54, -1 };
static const short CheckerArray54[] = { 30, -1 };
static const short SubPackageArray55[] = { 61, 56, 58, 57, 59, 60, -1 };
static const short CheckerArray56[] = { 22, -1 };
static const short CheckerArray57[] = { 40, -1 };
static const short CheckerArray58[] = { 37, -1 };
static const short CheckerArray59[] = { 55, -1 };
static const short CheckerArray60[] = { 61, -1 };
static const short SubPackageArray61[] = { 64, 62, 63, -1 };
static const short CheckerArray62[] = { 12, -1 };
static const short CheckerArray63[] = { 13, -1 };
static const short CheckerArray64[] = { 15, -1 };
static const short SubPackageArray65[] = { 66, -1 };
static const short CheckerArray66[] = { 33, -1 };
static const short SubPackageArray67[] = { 71, 82, 68, 69, 70, -1 };
static const short CheckerArray68[] = { 36, -1 };
static const short CheckerArray69[] = { 45, -1 };
static const short CheckerArray70[] = { 35, -1 };
static const short SubPackageArray71[] = { 72, 77, 73, 81, 75, 74, 80, 79, 76, 78, -1 };
static const short CheckerArray72[] = { 46, -1 };
static const short CheckerArray73[] = { 23, -1 };
static const short CheckerArray74[] = { 50, -1 };
static const short CheckerArray75[] = { 41, -1 };
static const short CheckerArray76[] = { 42, -1 };
static const short CheckerArray77[] = { 43, -1 };
static const short CheckerArray78[] = { 56, -1 };
static const short CheckerArray79[] = { 51, -1 };
static const short CheckerArray80[] = { 52, -1 };
static const short CheckerArray81[] = { 72, -1 };
static const short SubPackageArray82[] = { 86, 84, 85, 83, -1 };
static const short CheckerArray83[] = { 7, -1 };
static const short CheckerArray84[] = { 10, -1 };
static const short CheckerArray85[] = { 11, -1 };
static const short SubPackageArray86[] = { 88, 87, -1 };
static const short CheckerArray87[] = { 48, -1 };
static const short CheckerArray88[] = { 47, -1 };
static const short SubPackageArray89[] = { 91, 90, -1 };
static const short CheckerArray90[] = { 29, -1 };
static const short SubPackageArray91[] = { 94, 93, 96, 95, 97, 98, 99, 92, -1 };
static const short CheckerArray92[] = { 63, -1 };
static const short CheckerArray93[] = { 74, -1 };
static const short CheckerArray94[] = { 75, -1 };
static const short CheckerArray95[] = { 76, -1 };
static const short CheckerArray96[] = { 77, -1 };
static const short CheckerArray97[] = { 78, -1 };
static const short CheckerArray98[] = { 79, -1 };
static const short CheckerArray99[] = { 80, -1 };
static const short SubPackageArray100[] = { 103, 101, 102, -1 };
static const short CheckerArray101[] = { 69, -1 };
static const short CheckerArray102[] = { 39, -1 };
static const short SubPackageArray103[] = { 105, 104, -1 };
static const short CheckerArray104[] = { 16, -1 };
static const short CheckerArray105[] = { 14, -1 };
#endif // GET_MEMBER_ARRAYS


#ifdef GET_CHECKNAME_TABLE
  { "core",                                             0, SubPackageArray0, false },
  { "core.AdjustedReturnValue",                         CheckerArray1, 0, false },
  { "core.AttributeNonNull",                            CheckerArray2, 0, false },
  { "core.CallAndMessage",                              CheckerArray3, 0, false },
  { "core.DivideZero",                                  CheckerArray4, 0, false },
  { "core.NullDereference",                             CheckerArray5, 0, false },
  { "core.StackAddressEscape",                          CheckerArray6, 0, false },
  { "core.UndefinedBinaryOperatorResult",               CheckerArray7, 0, false },
  { "core.VLASize",                                     CheckerArray8, 0, false },
  { "core.builtin",                                     0, SubPackageArray9, false },
  { "core.builtin.BuiltinFunctions",                    CheckerArray10, 0, false },
  { "core.builtin.NoReturnFunctions",                   CheckerArray11, 0, false },
  { "core.uninitialized",                               0, SubPackageArray12, false },
  { "core.uninitialized.ArraySubscript",                CheckerArray13, 0, false },
  { "core.uninitialized.Assign",                        CheckerArray14, 0, false },
  { "core.uninitialized.Branch",                        CheckerArray15, 0, false },
  { "core.uninitialized.CapturedBlockVariable",         CheckerArray16, 0, false },
  { "core.uninitialized.UndefReturn",                   CheckerArray17, 0, false },
  { "cplusplus",                                        0, 0, false },
  { "deadcode",                                         0, SubPackageArray19, false },
  { "deadcode.DeadStores",                              CheckerArray20, 0, false },
  { "debug",                                            0, SubPackageArray21, false },
  { "debug.DumpCFG",                                    CheckerArray22, 0, false },
  { "debug.DumpCallGraph",                              CheckerArray23, 0, false },
  { "debug.DumpDominators",                             CheckerArray24, 0, false },
  { "debug.DumpLiveVars",                               CheckerArray25, 0, false },
  { "debug.Stats",                                      CheckerArray26, 0, false },
  { "debug.TaintTest",                                  CheckerArray27, 0, false },
  { "debug.ViewCFG",                                    CheckerArray28, 0, false },
  { "debug.ViewCallGraph",                              CheckerArray29, 0, false },
  { "experimental",                                     0, SubPackageArray30, false },
  { "experimental.core",                                0, SubPackageArray31, true },
  { "experimental.core.BoolAssignment",                 CheckerArray32, 0, false },
  { "experimental.core.CastSize",                       CheckerArray33, 0, false },
  { "experimental.core.CastToStruct",                   CheckerArray34, 0, false },
  { "experimental.core.FixedAddr",                      CheckerArray35, 0, false },
  { "experimental.core.PointerArithm",                  CheckerArray36, 0, false },
  { "experimental.core.PointerSub",                     CheckerArray37, 0, false },
  { "experimental.core.SizeofPtr",                      CheckerArray38, 0, false },
  { "experimental.cplusplus",                           0, SubPackageArray39, true },
  { "experimental.cplusplus.Iterators",                 CheckerArray40, 0, false },
  { "experimental.cplusplus.VirtualCall",               CheckerArray41, 0, false },
  { "experimental.deadcode",                            0, SubPackageArray42, true },
  { "experimental.deadcode.IdempotentOperations",       CheckerArray43, 0, false },
  { "experimental.deadcode.UnreachableCode",            CheckerArray44, 0, false },
  { "experimental.osx",                                 0, SubPackageArray45, true },
  { "experimental.osx.cocoa",                           0, SubPackageArray46, true },
  { "experimental.osx.cocoa.Dealloc",                   CheckerArray47, 0, false },
  { "experimental.security",                            0, SubPackageArray48, true },
  { "experimental.security.ArrayBound",                 CheckerArray49, 0, false },
  { "experimental.security.ArrayBoundV2",               CheckerArray50, 0, false },
  { "experimental.security.MallocOverflow",             CheckerArray51, 0, false },
  { "experimental.security.ReturnPtrRange",             CheckerArray52, 0, false },
  { "experimental.security.taint",                      0, SubPackageArray53, true },
  { "experimental.security.taint.TaintPropagation",     CheckerArray54, 0, false },
  { "experimental.unix",                                0, SubPackageArray55, true },
  { "experimental.unix.Chroot",                         CheckerArray56, 0, false },
  { "experimental.unix.MallocSizeof",                   CheckerArray57, 0, false },
  { "experimental.unix.MallocWithAnnotations",          CheckerArray58, 0, false },
  { "experimental.unix.PthreadLock",                    CheckerArray59, 0, false },
  { "experimental.unix.Stream",                         CheckerArray60, 0, false },
  { "experimental.unix.cstring",                        0, SubPackageArray61, true },
  { "experimental.unix.cstring.BufferOverlap",          CheckerArray62, 0, false },
  { "experimental.unix.cstring.NotNullTerminated",      CheckerArray63, 0, false },
  { "experimental.unix.cstring.OutOfBounds",            CheckerArray64, 0, false },
  { "llvm",                                             0, SubPackageArray65, false },
  { "llvm.Conventions",                                 CheckerArray66, 0, false },
  { "osx",                                              0, SubPackageArray67, false },
  { "osx.API",                                          CheckerArray68, 0, false },
  { "osx.AtomicCAS",                                    CheckerArray69, 0, false },
  { "osx.SecKeychainAPI",                               CheckerArray70, 0, false },
  { "osx.cocoa",                                        0, SubPackageArray71, false },
  { "osx.cocoa.AtSync",                                 CheckerArray72, 0, false },
  { "osx.cocoa.ClassRelease",                           CheckerArray73, 0, false },
  { "osx.cocoa.IncompatibleMethodTypes",                CheckerArray74, 0, false },
  { "osx.cocoa.NSAutoreleasePool",                      CheckerArray75, 0, false },
  { "osx.cocoa.NSError",                                CheckerArray76, 0, false },
  { "osx.cocoa.NilArg",                                 CheckerArray77, 0, false },
  { "osx.cocoa.RetainCount",                            CheckerArray78, 0, false },
  { "osx.cocoa.SelfInit",                               CheckerArray79, 0, false },
  { "osx.cocoa.UnusedIvars",                            CheckerArray80, 0, false },
  { "osx.cocoa.VariadicMethodTypes",                    CheckerArray81, 0, false },
  { "osx.coreFoundation",                               0, SubPackageArray82, false },
  { "osx.coreFoundation.CFError",                       CheckerArray83, 0, false },
  { "osx.coreFoundation.CFNumber",                      CheckerArray84, 0, false },
  { "osx.coreFoundation.CFRetainRelease",               CheckerArray85, 0, false },
  { "osx.coreFoundation.containers",                    0, SubPackageArray86, false },
  { "osx.coreFoundation.containers.OutOfBounds",        CheckerArray87, 0, false },
  { "osx.coreFoundation.containers.PointerSizedValues", CheckerArray88, 0, false },
  { "security",                                         0, SubPackageArray89, false },
  { "security.FloatLoopCounter",                        CheckerArray90, 0, false },
  { "security.insecureAPI",                             0, SubPackageArray91, false },
  { "security.insecureAPI.UncheckedReturn",             CheckerArray92, 0, false },
  { "security.insecureAPI.getpw",                       CheckerArray93, 0, false },
  { "security.insecureAPI.gets",                        CheckerArray94, 0, false },
  { "security.insecureAPI.mkstemp",                     CheckerArray95, 0, false },
  { "security.insecureAPI.mktemp",                      CheckerArray96, 0, false },
  { "security.insecureAPI.rand",                        CheckerArray97, 0, false },
  { "security.insecureAPI.strcpy",                      CheckerArray98, 0, false },
  { "security.insecureAPI.vfork",                       CheckerArray99, 0, false },
  { "unix",                                             0, SubPackageArray100, false },
  { "unix.API",                                         CheckerArray101, 0, false },
  { "unix.Malloc",                                      CheckerArray102, 0, false },
  { "unix.cstring",                                     0, SubPackageArray103, true },
  { "unix.cstring.BadSizeArg",                          CheckerArray104, 0, false },
  { "unix.cstring.NullArg",                             CheckerArray105, 0, false },
#endif // GET_CHECKNAME_TABLE


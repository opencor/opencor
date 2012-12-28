
#ifdef GET_GROUPS
#endif // GET_GROUPS


#ifdef GET_PACKAGES
PACKAGE("alpha", -1, false)
PACKAGE("alpha.core", -1, true)
PACKAGE("alpha.cplusplus", -1, true)
PACKAGE("alpha.deadcode", -1, true)
PACKAGE("alpha.osx", -1, true)
PACKAGE("alpha.osx.cocoa", -1, true)
PACKAGE("alpha.security", -1, true)
PACKAGE("alpha.security.taint", -1, true)
PACKAGE("alpha.unix", -1, true)
PACKAGE("alpha.unix.cstring", -1, true)
PACKAGE("core", -1, false)
PACKAGE("core.builtin", -1, false)
PACKAGE("core.uninitialized", -1, false)
PACKAGE("cplusplus", -1, false)
PACKAGE("deadcode", -1, false)
PACKAGE("debug", -1, false)
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
CHECKER("debug.Stats", AnalyzerStatsChecker, AnalyzerStatsChecker.cpp, "Emit warnings with analyzer statistics", -1, false)
CHECKER("alpha.security.ArrayBound", ArrayBoundChecker, ArrayBoundChecker.cpp, "Warn about buffer overflows (older checker)", -1, true)
CHECKER("alpha.security.ArrayBoundV2", ArrayBoundCheckerV2, ArrayBoundCheckerV2.cpp, "Warn about buffer overflows (newer checker)", -1, true)
CHECKER("core.AttributeNonNull", AttrNonNullChecker, AttrNonNullChecker.cpp, "Check for null pointers passed as arguments to a function whose arguments are marked with the 'nonnull' attribute", -1, false)
CHECKER("alpha.core.BoolAssignment", BoolAssignmentChecker, BoolAssignmentChecker.cpp, "Warn about assigning non-{0,1} values to Boolean variables", -1, true)
CHECKER("core.builtin.BuiltinFunctions", BuiltinFunctionChecker, BuiltinFunctionChecker.cpp, "Evaluate compiler builtin functions (e.g., alloca())", -1, false)
CHECKER("osx.coreFoundation.CFError", CFErrorChecker, NSErrorChecker.cpp, "Check usage of CFErrorRef* parameters", -1, false)
CHECKER("debug.DumpCFG", CFGDumper, DebugCheckers.cpp, "Display Control-Flow Graphs", -1, false)
CHECKER("debug.ViewCFG", CFGViewer, DebugCheckers.cpp, "View Control-Flow Graphs using GraphViz", -1, false)
CHECKER("osx.coreFoundation.CFNumber", CFNumberCreateChecker, BasicObjCFoundationChecks.cpp, "Check for proper uses of CFNumberCreate", -1, false)
CHECKER("osx.coreFoundation.CFRetainRelease", CFRetainReleaseChecker, BasicObjCFoundationChecks.cpp, "Check for null arguments to CFRetain/CFRelease/CFMakeCollectable", -1, false)
CHECKER("alpha.unix.cstring.BufferOverlap", CStringBufferOverlap, CStringChecker.cpp, "Checks for overlap in two buffer arguments", -1, true)
CHECKER("alpha.unix.cstring.NotNullTerminated", CStringNotNullTerm, CStringChecker.cpp, "Check for arguments which are not null-terminating strings", -1, true)
CHECKER("unix.cstring.NullArg", CStringNullArg, CStringChecker.cpp, "Check for null pointers being passed as arguments to C string functions", -1, true)
CHECKER("alpha.unix.cstring.OutOfBounds", CStringOutOfBounds, CStringChecker.cpp, "Check for out-of-bounds access in string functions", -1, true)
CHECKER("unix.cstring.BadSizeArg", CStringSyntaxChecker, CStringSyntaxChecker.cpp, "Check the size argument passed into C string functions for common erroneous patterns", -1, true)
CHECKER("core.CallAndMessage", CallAndMessageChecker, CallAndMessageChecker.cpp, "Check for logical errors for function calls and Objective-C message expressions (e.g., uninitialized arguments, null function pointers)", -1, false)
CHECKER("debug.DumpCalls", CallDumper, TraversalChecker.cpp, "Print calls as they are traversed by the engine", -1, false)
CHECKER("debug.DumpCallGraph", CallGraphDumper, DebugCheckers.cpp, "Display Call Graph", -1, false)
CHECKER("debug.ViewCallGraph", CallGraphViewer, DebugCheckers.cpp, "View Call Graph using GraphViz", -1, false)
CHECKER("alpha.core.CastSize", CastSizeChecker, CastSizeChecker.cpp, "Check when casting a malloc'ed type T, whether the size is a multiple of the size of T", -1, true)
CHECKER("alpha.core.CastToStruct", CastToStructChecker, CastToStructChecker.cpp, "Check for cast from non-struct pointer to struct pointer", -1, true)
CHECKER("alpha.unix.Chroot", ChrootChecker, ChrootChecker.cpp, "Check improper use of chroot", -1, true)
CHECKER("osx.cocoa.ClassRelease", ClassReleaseChecker, BasicObjCFoundationChecks.cpp, "Check for sending 'retain', 'release', or 'autorelease' directly to a Class", -1, false)
CHECKER("debug.ConfigDumper", ConfigDumper, DebugCheckers.cpp, "Dump config table", -1, false)
CHECKER("deadcode.DeadStores", DeadStoresChecker, DeadStoresChecker.cpp, "Check for values stored to variables that are never read afterwards", -1, false)
CHECKER("core.NullDereference", DereferenceChecker, DereferenceChecker.cpp, "Check for dereferences of null pointers", -1, false)
CHECKER("alpha.osx.cocoa.DirectIvarAssignment", DirectIvarAssignment, DirectIvarAssignment.cpp, "Check that the invalidatable instance variables are invalidated in the methods annotated with objc_instance_variable_invalidator", -1, true)
CHECKER("core.DivideZero", DivZeroChecker, DivZeroChecker.cpp, "Check for division by zero", -1, false)
CHECKER("debug.DumpDominators", DominatorsTreeDumper, DebugCheckers.cpp, "Print the dominance tree for a given CFG", -1, false)
CHECKER("core.DynamicTypePropagation", DynamicTypePropagation, DynamicTypePropagation.cpp, "Generate dynamic type information", -1, false)
CHECKER("debug.ExprInspection", ExprInspectionChecker, ExprInspectionChecker.cpp, "Check the analyzer's understanding of expressions", -1, false)
CHECKER("alpha.core.FixedAddr", FixedAddressChecker, FixedAddressChecker.cpp, "Check for assignment of a fixed address to a pointer", -1, true)
CHECKER("security.FloatLoopCounter", FloatLoopCounter, CheckSecuritySyntaxOnly.cpp, "Warn on using a floating point value as a loop counter (CERT: FLP30-C, FLP30-CPP)", -1, false)
CHECKER("alpha.security.taint.TaintPropagation", GenericTaintChecker, GenericTaintChecker.cpp, "Generate taint information used by other checkers", -1, true)
CHECKER("alpha.deadcode.IdempotentOperations", IdempotentOperationChecker, IdempotentOperationChecker.cpp, "Warn about idempotent operations", -1, true)
CHECKER("alpha.osx.cocoa.InstanceVariableInvalidation", IvarInvalidationChecker, IvarInvalidationChecker.cpp, "Check that the invalidatable instance variables are invalidated in the methods annotated with objc_instance_variable_invalidator", -1, true)
CHECKER("llvm.Conventions", LLVMConventionsChecker, LLVMConventionsChecker.cpp, "Check code for LLVM codebase conventions", -1, false)
CHECKER("debug.DumpLiveVars", LiveVariablesDumper, DebugCheckers.cpp, "Print results of live variable analysis", -1, false)
CHECKER("osx.SecKeychainAPI", MacOSKeychainAPIChecker, MacOSKeychainAPIChecker.cpp, "Check for proper uses of Secure Keychain APIs", -1, false)
CHECKER("osx.API", MacOSXAPIChecker, MacOSXAPIChecker.cpp, "Check for proper uses of various Mac OS X APIs", -1, false)
CHECKER("alpha.unix.MallocWithAnnotations", MallocOptimistic, MallocChecker.cpp, "Check for memory leaks, double free, and use-after-free problems. Assumes that all user-defined functions which might free a pointer are annotated.", -1, true)
CHECKER("alpha.security.MallocOverflow", MallocOverflowSecurityChecker, MallocOverflowSecurityChecker.cpp, "Check for overflows in the arguments to malloc()", -1, true)
CHECKER("unix.Malloc", MallocPessimistic, MallocChecker.cpp, "Check for memory leaks, double free, and use-after-free problems.", -1, false)
CHECKER("unix.MallocSizeof", MallocSizeofChecker, MallocSizeofChecker.cpp, "Check for dubious malloc arguments involving sizeof", -1, false)
CHECKER("osx.cocoa.NSAutoreleasePool", NSAutoreleasePoolChecker, NSAutoreleasePoolChecker.cpp, "Warn for suboptimal uses of NSAutoreleasePool in Objective-C GC mode", -1, false)
CHECKER("osx.cocoa.NSError", NSErrorChecker, NSErrorChecker.cpp, "Check usage of NSError** parameters", -1, false)
CHECKER("osx.cocoa.NilArg", NilArgChecker, BasicObjCFoundationChecks.cpp, "Check for prohibited nil arguments to ObjC method calls", -1, false)
CHECKER("core.builtin.NoReturnFunctions", NoReturnFunctionChecker, NoReturnFunctionChecker.cpp, "Evaluate \"panic\" functions that are known to not return to the caller", -1, false)
CHECKER("osx.cocoa.AtSync", ObjCAtSyncChecker, ObjCAtSyncChecker.cpp, "Check for nil pointers used as mutexes for @synchronized", -1, false)
CHECKER("osx.coreFoundation.containers.PointerSizedValues", ObjCContainersASTChecker, ObjCContainersASTChecker.cpp, "Warns if 'CFArray', 'CFDictionary', 'CFSet' are created with non-pointer-size values", -1, false)
CHECKER("osx.coreFoundation.containers.OutOfBounds", ObjCContainersChecker, ObjCContainersChecker.cpp, "Checks for index out-of-bounds when using 'CFArray' API", -1, false)
CHECKER("alpha.osx.cocoa.Dealloc", ObjCDeallocChecker, CheckObjCDealloc.cpp, "Warn about Objective-C classes that lack a correct implementation of -dealloc", -1, true)
CHECKER("osx.cocoa.Loops", ObjCLoopChecker, BasicObjCFoundationChecks.cpp, "Improved modeling of loops using Cocoa collection types", -1, false)
CHECKER("osx.cocoa.IncompatibleMethodTypes", ObjCMethSigsChecker, CheckObjCInstMethSignature.cpp, "Warn about Objective-C method signatures with type incompatibilities", -1, false)
CHECKER("osx.cocoa.NonNilReturnValue", ObjCNonNilReturnValueChecker, BasicObjCFoundationChecks.cpp, "Model the APIs that are guaranteed to return a non-nil value", -1, false)
CHECKER("osx.cocoa.SelfInit", ObjCSelfInitChecker, ObjCSelfInitChecker.cpp, "Check that 'self' is properly initialized inside an initializer method", -1, false)
CHECKER("alpha.osx.cocoa.MissingSuperCall", ObjCSuperCallChecker, ObjCMissingSuperCallChecker.cpp, "Warn about Objective-C methods that lack a necessary call to super", -1, true)
CHECKER("osx.cocoa.UnusedIvars", ObjCUnusedIvarsChecker, ObjCUnusedIVarsChecker.cpp, "Warn about private ivars that are never used", -1, false)
CHECKER("alpha.core.PointerArithm", PointerArithChecker, PointerArithChecker, "Check for pointer arithmetic on locations other than array elements", -1, true)
CHECKER("alpha.core.PointerSub", PointerSubChecker, PointerSubChecker, "Check for pointer subtractions on two pointers pointing to different memory chunks", -1, true)
CHECKER("alpha.unix.PthreadLock", PthreadLockChecker, PthreadLockChecker.cpp, "Simple lock -> unlock checker", -1, true)
CHECKER("osx.cocoa.RetainCount", RetainCountChecker, RetainCountChecker.cpp, "Check for leaks and improper reference count management", -1, false)
CHECKER("alpha.security.ReturnPtrRange", ReturnPointerRangeChecker, ReturnPointerRangeChecker.cpp, "Check for an out-of-bound pointer being returned to callers", -1, true)
CHECKER("core.uninitialized.UndefReturn", ReturnUndefChecker, ReturnUndefChecker.cpp, "Check for uninitialized values being returned to the caller", -1, false)
CHECKER("alpha.unix.SimpleStream", SimpleStreamChecker, SimpleStreamChecker.cpp, "Check for misuses of stream APIs", -1, true)
CHECKER("alpha.core.SizeofPtr", SizeofPointerChecker, CheckSizeofPointer.cpp, "Warn about unintended use of sizeof() on pointer expressions", -1, true)
CHECKER("core.StackAddressEscape", StackAddrEscapeChecker, StackAddrEscapeChecker.cpp, "Check that addresses to stack memory do not escape the function", -1, false)
CHECKER("alpha.unix.Stream", StreamChecker, StreamChecker.cpp, "Check stream handling functions", -1, true)
CHECKER("debug.TaintTest", TaintTesterChecker, TaintTesterChecker.cpp, "Mark tainted symbols as such.", -1, false)
CHECKER("debug.DumpTraversal", TraversalDumper, TraversalChecker.cpp, "Print branch conditions as they are traversed by the engine", -1, false)
CHECKER("security.insecureAPI.UncheckedReturn", UncheckedReturn, CheckSecuritySyntaxOnly.cpp, "Warn on uses of functions whose return values must be always checked", -1, false)
CHECKER("core.uninitialized.Branch", UndefBranchChecker, UndefBranchChecker.cpp, "Check for uninitialized values used as branch conditions", -1, false)
CHECKER("core.uninitialized.CapturedBlockVariable", UndefCapturedBlockVarChecker, UndefCapturedBlockVarChecker.cpp, "Check for blocks that capture uninitialized values", -1, false)
CHECKER("core.UndefinedBinaryOperatorResult", UndefResultChecker, UndefResultChecker.cpp, "Check for undefined results of binary operators", -1, false)
CHECKER("core.uninitialized.ArraySubscript", UndefinedArraySubscriptChecker, UndefinedArraySubscriptChecker.cpp, "Check for uninitialized values used as array subscripts", -1, false)
CHECKER("core.uninitialized.Assign", UndefinedAssignmentChecker, UndefinedAssignmentChecker.cpp, "Check for assigning uninitialized values", -1, false)
CHECKER("unix.API", UnixAPIChecker, UnixAPIChecker.cpp, "Check calls to various UNIX/Posix functions", -1, false)
CHECKER("alpha.deadcode.UnreachableCode", UnreachableCodeChecker, UnreachableCodeChecker.cpp, "Check unreachable code", -1, true)
CHECKER("core.VLASize", VLASizeChecker, VLASizeChecker.cpp, "Check for declarations of VLA of undefined or zero size", -1, false)
CHECKER("osx.cocoa.VariadicMethodTypes", VariadicMethodTypeChecker, BasicObjCFoundationChecks.cpp, "Check for passing non-Objective-C types to variadic collection initialization methods that expect only Objective-C types", -1, false)
CHECKER("alpha.cplusplus.VirtualCall", VirtualCallChecker, VirtualCallChecker.cpp, "Check virtual function calls during construction or destruction", -1, true)
CHECKER("security.insecureAPI.getpw", getpw, CheckSecuritySyntaxOnly.cpp, "Warn on uses of the 'getpw' function", -1, false)
CHECKER("security.insecureAPI.gets", gets, CheckSecuritySyntaxOnly.cpp, "Warn on uses of the 'gets' function", -1, false)
CHECKER("security.insecureAPI.mkstemp", mkstemp, CheckSecuritySyntaxOnly.cpp, "Warn when 'mkstemp' is passed fewer than 6 X's in the format string", -1, false)
CHECKER("security.insecureAPI.mktemp", mktemp, CheckSecuritySyntaxOnly.cpp, "Warn on uses of the 'mktemp' function", -1, false)
CHECKER("security.insecureAPI.rand", rand, CheckSecuritySyntaxOnly.cpp, "Warn on uses of the 'rand', 'random', and related functions", -1, false)
CHECKER("security.insecureAPI.strcpy", strcpy, CheckSecuritySyntaxOnly.cpp, "Warn on uses of the 'strcpy' and 'strcat' functions", -1, false)
CHECKER("security.insecureAPI.vfork", vfork, CheckSecuritySyntaxOnly.cpp, "Warn on uses of the 'vfork' function", -1, false)
#endif // GET_CHECKERS


#ifdef GET_MEMBER_ARRAYS
static const short SubPackageArray0[] = { 1, 9, 11, 20, 27, 14, -1 };
static const short SubPackageArray1[] = { 2, 3, 4, 5, 6, 7, 8, -1 };
static const short CheckerArray2[] = { 4, -1 };
static const short CheckerArray3[] = { 20, -1 };
static const short CheckerArray4[] = { 21, -1 };
static const short CheckerArray5[] = { 32, -1 };
static const short CheckerArray6[] = { 59, -1 };
static const short CheckerArray7[] = { 60, -1 };
static const short CheckerArray8[] = { 66, -1 };
static const short SubPackageArray9[] = { 10, -1 };
static const short CheckerArray10[] = { 81, -1 };
static const short SubPackageArray11[] = { 12, 13, -1 };
static const short CheckerArray12[] = { 35, -1 };
static const short CheckerArray13[] = { 78, -1 };
static const short SubPackageArray14[] = { 15, -1 };
static const short SubPackageArray15[] = { 16, 18, 17, 19, -1 };
static const short CheckerArray16[] = { 52, -1 };
static const short CheckerArray17[] = { 27, -1 };
static const short CheckerArray18[] = { 36, -1 };
static const short CheckerArray19[] = { 57, -1 };
static const short SubPackageArray20[] = { 25, 21, 22, 24, 23, -1 };
static const short CheckerArray21[] = { 1, -1 };
static const short CheckerArray22[] = { 2, -1 };
static const short CheckerArray23[] = { 42, -1 };
static const short CheckerArray24[] = { 63, -1 };
static const short SubPackageArray25[] = { 26, -1 };
static const short CheckerArray26[] = { 34, -1 };
static const short SubPackageArray27[] = { 33, 28, 29, 30, 32, 31, -1 };
static const short CheckerArray28[] = { 22, -1 };
static const short CheckerArray29[] = { 41, -1 };
static const short CheckerArray30[] = { 61, -1 };
static const short CheckerArray31[] = { 65, -1 };
static const short CheckerArray32[] = { 68, -1 };
static const short SubPackageArray33[] = { 36, 34, 35, -1 };
static const short CheckerArray34[] = { 11, -1 };
static const short CheckerArray35[] = { 12, -1 };
static const short CheckerArray36[] = { 14, -1 };
static const short SubPackageArray37[] = { 46, 49, 42, 39, 38, 45, 40, 44, 43, 41, -1 };
static const short CheckerArray38[] = { 3, -1 };
static const short CheckerArray39[] = { 16, -1 };
static const short CheckerArray40[] = { 28, -1 };
static const short CheckerArray41[] = { 30, -1 };
static const short CheckerArray42[] = { 26, -1 };
static const short CheckerArray43[] = { 67, -1 };
static const short CheckerArray44[] = { 74, -1 };
static const short CheckerArray45[] = { 79, -1 };
static const short SubPackageArray46[] = { 48, 47, -1 };
static const short CheckerArray47[] = { 5, -1 };
static const short CheckerArray48[] = { 48, -1 };
static const short SubPackageArray49[] = { 50, 51, 52, 53, 54, -1 };
static const short CheckerArray50[] = { 75, -1 };
static const short CheckerArray51[] = { 76, -1 };
static const short CheckerArray52[] = { 72, -1 };
static const short CheckerArray53[] = { 73, -1 };
static const short CheckerArray54[] = { 64, -1 };
static const short SubPackageArray56[] = { 57, -1 };
static const short CheckerArray57[] = { 25, -1 };
static const short SubPackageArray58[] = { 63, 64, 69, 60, 70, 61, 59, 65, 62, 67, 68, 66, -1 };
static const short CheckerArray59[] = { 24, -1 };
static const short CheckerArray60[] = { 7, -1 };
static const short CheckerArray61[] = { 18, -1 };
static const short CheckerArray62[] = { 17, -1 };
static const short CheckerArray63[] = { 29, -1 };
static const short CheckerArray64[] = { 38, -1 };
static const short CheckerArray65[] = { 70, -1 };
static const short CheckerArray66[] = { 31, -1 };
static const short CheckerArray67[] = { 0, -1 };
static const short CheckerArray68[] = { 69, -1 };
static const short CheckerArray69[] = { 8, -1 };
static const short CheckerArray70[] = { 19, -1 };
static const short SubPackageArray71[] = { 72, -1 };
static const short CheckerArray72[] = { 37, -1 };
static const short SubPackageArray73[] = { 76, 89, 74, 75, -1 };
static const short CheckerArray74[] = { 40, -1 };
static const short CheckerArray75[] = { 39, -1 };
static const short SubPackageArray76[] = { 77, 83, 78, 88, 81, 79, 87, 86, 80, 84, 82, 85, -1 };
static const short CheckerArray77[] = { 49, -1 };
static const short CheckerArray78[] = { 23, -1 };
static const short CheckerArray79[] = { 54, -1 };
static const short CheckerArray80[] = { 53, -1 };
static const short CheckerArray81[] = { 45, -1 };
static const short CheckerArray82[] = { 46, -1 };
static const short CheckerArray83[] = { 47, -1 };
static const short CheckerArray84[] = { 55, -1 };
static const short CheckerArray85[] = { 62, -1 };
static const short CheckerArray86[] = { 56, -1 };
static const short CheckerArray87[] = { 58, -1 };
static const short CheckerArray88[] = { 80, -1 };
static const short SubPackageArray89[] = { 93, 91, 92, 90, -1 };
static const short CheckerArray90[] = { 6, -1 };
static const short CheckerArray91[] = { 9, -1 };
static const short CheckerArray92[] = { 10, -1 };
static const short SubPackageArray93[] = { 95, 94, -1 };
static const short CheckerArray94[] = { 51, -1 };
static const short CheckerArray95[] = { 50, -1 };
static const short SubPackageArray96[] = { 98, 97, -1 };
static const short CheckerArray97[] = { 33, -1 };
static const short SubPackageArray98[] = { 101, 100, 103, 102, 104, 105, 106, 99, -1 };
static const short CheckerArray99[] = { 71, -1 };
static const short CheckerArray100[] = { 82, -1 };
static const short CheckerArray101[] = { 83, -1 };
static const short CheckerArray102[] = { 84, -1 };
static const short CheckerArray103[] = { 85, -1 };
static const short CheckerArray104[] = { 86, -1 };
static const short CheckerArray105[] = { 87, -1 };
static const short CheckerArray106[] = { 88, -1 };
static const short SubPackageArray107[] = { 111, 108, 109, 110, -1 };
static const short CheckerArray108[] = { 77, -1 };
static const short CheckerArray109[] = { 43, -1 };
static const short CheckerArray110[] = { 44, -1 };
static const short SubPackageArray111[] = { 113, 112, -1 };
static const short CheckerArray112[] = { 15, -1 };
static const short CheckerArray113[] = { 13, -1 };
#endif // GET_MEMBER_ARRAYS


#ifdef GET_CHECKNAME_TABLE
  { "alpha",                                            0, SubPackageArray0, false },
  { "alpha.core",                                       0, SubPackageArray1, true },
  { "alpha.core.BoolAssignment",                        CheckerArray2, 0, false },
  { "alpha.core.CastSize",                              CheckerArray3, 0, false },
  { "alpha.core.CastToStruct",                          CheckerArray4, 0, false },
  { "alpha.core.FixedAddr",                             CheckerArray5, 0, false },
  { "alpha.core.PointerArithm",                         CheckerArray6, 0, false },
  { "alpha.core.PointerSub",                            CheckerArray7, 0, false },
  { "alpha.core.SizeofPtr",                             CheckerArray8, 0, false },
  { "alpha.cplusplus",                                  0, SubPackageArray9, true },
  { "alpha.cplusplus.VirtualCall",                      CheckerArray10, 0, false },
  { "alpha.deadcode",                                   0, SubPackageArray11, true },
  { "alpha.deadcode.IdempotentOperations",              CheckerArray12, 0, false },
  { "alpha.deadcode.UnreachableCode",                   CheckerArray13, 0, false },
  { "alpha.osx",                                        0, SubPackageArray14, true },
  { "alpha.osx.cocoa",                                  0, SubPackageArray15, true },
  { "alpha.osx.cocoa.Dealloc",                          CheckerArray16, 0, false },
  { "alpha.osx.cocoa.DirectIvarAssignment",             CheckerArray17, 0, false },
  { "alpha.osx.cocoa.InstanceVariableInvalidation",     CheckerArray18, 0, false },
  { "alpha.osx.cocoa.MissingSuperCall",                 CheckerArray19, 0, false },
  { "alpha.security",                                   0, SubPackageArray20, true },
  { "alpha.security.ArrayBound",                        CheckerArray21, 0, false },
  { "alpha.security.ArrayBoundV2",                      CheckerArray22, 0, false },
  { "alpha.security.MallocOverflow",                    CheckerArray23, 0, false },
  { "alpha.security.ReturnPtrRange",                    CheckerArray24, 0, false },
  { "alpha.security.taint",                             0, SubPackageArray25, true },
  { "alpha.security.taint.TaintPropagation",            CheckerArray26, 0, false },
  { "alpha.unix",                                       0, SubPackageArray27, true },
  { "alpha.unix.Chroot",                                CheckerArray28, 0, false },
  { "alpha.unix.MallocWithAnnotations",                 CheckerArray29, 0, false },
  { "alpha.unix.PthreadLock",                           CheckerArray30, 0, false },
  { "alpha.unix.SimpleStream",                          CheckerArray31, 0, false },
  { "alpha.unix.Stream",                                CheckerArray32, 0, false },
  { "alpha.unix.cstring",                               0, SubPackageArray33, true },
  { "alpha.unix.cstring.BufferOverlap",                 CheckerArray34, 0, false },
  { "alpha.unix.cstring.NotNullTerminated",             CheckerArray35, 0, false },
  { "alpha.unix.cstring.OutOfBounds",                   CheckerArray36, 0, false },
  { "core",                                             0, SubPackageArray37, false },
  { "core.AttributeNonNull",                            CheckerArray38, 0, false },
  { "core.CallAndMessage",                              CheckerArray39, 0, false },
  { "core.DivideZero",                                  CheckerArray40, 0, false },
  { "core.DynamicTypePropagation",                      CheckerArray41, 0, false },
  { "core.NullDereference",                             CheckerArray42, 0, false },
  { "core.StackAddressEscape",                          CheckerArray43, 0, false },
  { "core.UndefinedBinaryOperatorResult",               CheckerArray44, 0, false },
  { "core.VLASize",                                     CheckerArray45, 0, false },
  { "core.builtin",                                     0, SubPackageArray46, false },
  { "core.builtin.BuiltinFunctions",                    CheckerArray47, 0, false },
  { "core.builtin.NoReturnFunctions",                   CheckerArray48, 0, false },
  { "core.uninitialized",                               0, SubPackageArray49, false },
  { "core.uninitialized.ArraySubscript",                CheckerArray50, 0, false },
  { "core.uninitialized.Assign",                        CheckerArray51, 0, false },
  { "core.uninitialized.Branch",                        CheckerArray52, 0, false },
  { "core.uninitialized.CapturedBlockVariable",         CheckerArray53, 0, false },
  { "core.uninitialized.UndefReturn",                   CheckerArray54, 0, false },
  { "cplusplus",                                        0, 0, false },
  { "deadcode",                                         0, SubPackageArray56, false },
  { "deadcode.DeadStores",                              CheckerArray57, 0, false },
  { "debug",                                            0, SubPackageArray58, false },
  { "debug.ConfigDumper",                               CheckerArray59, 0, false },
  { "debug.DumpCFG",                                    CheckerArray60, 0, false },
  { "debug.DumpCallGraph",                              CheckerArray61, 0, false },
  { "debug.DumpCalls",                                  CheckerArray62, 0, false },
  { "debug.DumpDominators",                             CheckerArray63, 0, false },
  { "debug.DumpLiveVars",                               CheckerArray64, 0, false },
  { "debug.DumpTraversal",                              CheckerArray65, 0, false },
  { "debug.ExprInspection",                             CheckerArray66, 0, false },
  { "debug.Stats",                                      CheckerArray67, 0, false },
  { "debug.TaintTest",                                  CheckerArray68, 0, false },
  { "debug.ViewCFG",                                    CheckerArray69, 0, false },
  { "debug.ViewCallGraph",                              CheckerArray70, 0, false },
  { "llvm",                                             0, SubPackageArray71, false },
  { "llvm.Conventions",                                 CheckerArray72, 0, false },
  { "osx",                                              0, SubPackageArray73, false },
  { "osx.API",                                          CheckerArray74, 0, false },
  { "osx.SecKeychainAPI",                               CheckerArray75, 0, false },
  { "osx.cocoa",                                        0, SubPackageArray76, false },
  { "osx.cocoa.AtSync",                                 CheckerArray77, 0, false },
  { "osx.cocoa.ClassRelease",                           CheckerArray78, 0, false },
  { "osx.cocoa.IncompatibleMethodTypes",                CheckerArray79, 0, false },
  { "osx.cocoa.Loops",                                  CheckerArray80, 0, false },
  { "osx.cocoa.NSAutoreleasePool",                      CheckerArray81, 0, false },
  { "osx.cocoa.NSError",                                CheckerArray82, 0, false },
  { "osx.cocoa.NilArg",                                 CheckerArray83, 0, false },
  { "osx.cocoa.NonNilReturnValue",                      CheckerArray84, 0, false },
  { "osx.cocoa.RetainCount",                            CheckerArray85, 0, false },
  { "osx.cocoa.SelfInit",                               CheckerArray86, 0, false },
  { "osx.cocoa.UnusedIvars",                            CheckerArray87, 0, false },
  { "osx.cocoa.VariadicMethodTypes",                    CheckerArray88, 0, false },
  { "osx.coreFoundation",                               0, SubPackageArray89, false },
  { "osx.coreFoundation.CFError",                       CheckerArray90, 0, false },
  { "osx.coreFoundation.CFNumber",                      CheckerArray91, 0, false },
  { "osx.coreFoundation.CFRetainRelease",               CheckerArray92, 0, false },
  { "osx.coreFoundation.containers",                    0, SubPackageArray93, false },
  { "osx.coreFoundation.containers.OutOfBounds",        CheckerArray94, 0, false },
  { "osx.coreFoundation.containers.PointerSizedValues", CheckerArray95, 0, false },
  { "security",                                         0, SubPackageArray96, false },
  { "security.FloatLoopCounter",                        CheckerArray97, 0, false },
  { "security.insecureAPI",                             0, SubPackageArray98, false },
  { "security.insecureAPI.UncheckedReturn",             CheckerArray99, 0, false },
  { "security.insecureAPI.getpw",                       CheckerArray100, 0, false },
  { "security.insecureAPI.gets",                        CheckerArray101, 0, false },
  { "security.insecureAPI.mkstemp",                     CheckerArray102, 0, false },
  { "security.insecureAPI.mktemp",                      CheckerArray103, 0, false },
  { "security.insecureAPI.rand",                        CheckerArray104, 0, false },
  { "security.insecureAPI.strcpy",                      CheckerArray105, 0, false },
  { "security.insecureAPI.vfork",                       CheckerArray106, 0, false },
  { "unix",                                             0, SubPackageArray107, false },
  { "unix.API",                                         CheckerArray108, 0, false },
  { "unix.Malloc",                                      CheckerArray109, 0, false },
  { "unix.MallocSizeof",                                CheckerArray110, 0, false },
  { "unix.cstring",                                     0, SubPackageArray111, true },
  { "unix.cstring.BadSizeArg",                          CheckerArray112, 0, false },
  { "unix.cstring.NullArg",                             CheckerArray113, 0, false },
#endif // GET_CHECKNAME_TABLE

